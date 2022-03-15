
#include "virtualMemorySpace.h"

#ifdef WINBUILD
#include <Windows.h>
#endif // WINBUILD

#include <stdio.h>

virtualMemorySpace::virtualMemorySpace()//if you don't need a memory offset then don't worry about it.
{
    memorySize=0;
    memoryOffset=0;
    top=nullptr;
    islooped = MEMSPACE_LOOP_DEFAULT;
}

virtualMemorySpace::virtualMemorySpace(int memoffset)
{
    memorySize=0;
    memoryOffset = memoffset;
    top=nullptr;
    islooped = MEMSPACE_LOOP_DEFAULT;
}

virtualMemorySpace::~virtualMemorySpace()
{
    while(memorySize > 0)
    {
        removePage();
    }
}

void virtualMemorySpace::virtualMemorySpace::recalcSize()//check the linked list to see how much memory we are working with.
{
    PageAddresses.clear();
    PageList.clear();

    memorySize = 0;
    if(top == nullptr){return;};
    addressSpace* _search = top;
    while(_search != nullptr)// loop ends when the pointer to _search is set to zero through the last iterations _search->_next being zero.
    {
        PageAddresses.push_back(memorySize);
        PageList.push_back(_search);
        memorySize += _search->_size();
        _search = _search->_next;
    }
    return;
}

addressSpace* virtualMemorySpace::_getAddressPage(unsigned long int addr,unsigned long int* offset) //returns NULL if the address is out of scope of this memory space.
{
    /**
    returns a pointer to the page that the address is in.
    and the offset is how many bytes from the start of the page that the address is at.
    **/

    addr -= memoryOffset;
        //adjusting for memory offset
        /*
            This is supposed to be for memory spaces that don't start at zero.
            so by subtracting our virtual offset, internally we can start at zero,
            while to the _getAddressPage function it can appear as if we start at the virtual offset.
        */


    if(islooped)
    {
        addr%=memorySize;
    }
    else
    {
        if(addr >= memorySize){return NULL;};//address is outside of the scope of this memory space;
    }

    addressSpace* _search = top;
    //if(_search == NULL){return NULL;}

    while(_search != NULL) // loop ends when the pointer to _search is set to NULL through the last iterations _search->_next being NULL.
    {
        if(addr < _search->_size())
        {
            *offset = addr;
            return _search;
        }
        addr -= _search->_size(); // If you subtract a page's size from an address you get the address from the point of view of the next page
        _search = _search->_next;
    }
    return NULL; //if not found return a null pointer.
}

//void* getAddressPointer(unsigned long int addr); //in an arbitrary memory model, direct address pointers aren't possible, only ones that refer to the virtual memory model.

void virtualMemorySpace::addPage(const unsigned int Size)
{
    if(top == NULL) //if we don't have any pages
    {
        top = new virtualPage(NULL,Size); //make this the first page.
        bottom = top; // the only page is also the last page
    }
    else
    {
        bottom = new virtualPage(bottom,Size);
    }
    recalcSize();
}

void virtualMemorySpace::removePage()
{
    if(!bottom) //if we don't have any pages
    {
        return;
    }
    else
    {
        if(bottom == top) //this is our only page
        {
            top->cleanupAndUnlink();
            top = NULL;
            bottom = NULL;
        }
        else
        {
            #ifdef DBG_MESSAGES
            printf("removing page:n \n");
            #endif
            addressSpace* _tmp = bottom;
            bottom = _tmp->_last;
            bottom->_next = NULL;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = top;
            while(_search != NULL)
            {
                _search->_bottom = bottom;
                _search = _search->_next;
            }
            _tmp->cleanupAndUnlink();
        }
    }
    recalcSize();
}

void virtualMemorySpace::addAddressSpace(addressSpace* AS)
{
    if(top == NULL) //if we don't have any pages
    {
        top = AS; //make this the first page.
        bottom = top; // the only page is also the last page

        AS->_last = NULL; //this is our only page so no page before or after
        AS->_next = NULL;
        AS->_top = AS;
    }
    else
    {
        addressSpace* _tmpaddr = bottom;
        bottom = AS;
        AS->_last = _tmpaddr;
        _tmpaddr->_next = AS;
        AS->_next = NULL;
    }

    //pages need to be informed what the new bottom of the list is.
    addressSpace* _search = top;
    while(_search != NULL)
    {
        _search->_bottom = bottom;
        _search = _search->_next;
    }

    recalcSize();
}



void virtualMemorySpace::_getPageListInAddressRange( long unsigned int lowAddress, long unsigned int highAddress, addressSpace** pagelist, unsigned int* pagecount)
{

    if(top == NULL) //if we don't have any pages
    {
        *pagecount = 0; //let the caller know there are no pages.
        return;
    }

    lowAddress -= memoryOffset;  //when we receive addresses from the outside we subtract the virtual offset.
    highAddress -= memoryOffset; //in other functions that return an address we have to add that offset back.
                                 //this allows us some memory remapping functionality.

    addressSpace* tmp_AS; //placeholder for comparing pointers.
    *pagecount = 0; //the number of pages crossed while scanning.
    unsigned long int tmp_PageOffset = 0; //holds the returned offset within the page that the low address is in.

    #ifdef EXTRA_DEBUG_MESSAGES
    unsigned long int addressDifference = highAddress - lowAddress; //how many bytes to go through.
    printf("_getPageListInAddressRange\n");
    printf("  addressDif:%i\n",(int)addressDifference); //for checking our work so far
    #endif

    pagelist[0] = NULL; //has to be something that is not a valid pointer.

    while(lowAddress<=highAddress)
    {
        tmp_AS = _getAddressPage(lowAddress, &tmp_PageOffset);

        #ifdef EXTRA_DEBUG_MESSAGES
        printf("  tmp_AS_Size:%i\n",(int)tmp_AS->_size());
        printf("  tmpPageOffs:%i\n",(int)tmp_PageOffset);
        printf("  pageCount:%i\n",(int)*pagecount);
        #endif

        if(pagelist[*pagecount] != tmp_AS) //if this page isn't the same as the last page.
        {
            *pagecount += 1; //increase the number of pages.
            pagelist[*pagecount] = tmp_AS; //and add the new page to the list.

            lowAddress += tmp_AS->_size()-tmp_PageOffset; //skip this page.
                //the rest of the page is the size of the page minus the offset within the page that our address is at.
        }
        lowAddress += 1; //go to the next byte.
    }
    return;
}


addressHelper virtualMemorySpace::isAddressInPage(unsigned long int addr,int page)
{
    addressHelper AH;
    if(page+1 > pageCount())
    {
        AH.isInPage = 0;
        AH.offsetFromStartOfPage = 0;
        AH.lengthTillEndOfPage = 0;
        return AH;
    }
    unsigned long int pageSize = PageList[page]->_size();
    AH.isInPage = (addr > PageAddresses[page])&&(addr < PageAddresses[page]+pageSize);
    AH.offsetFromStartOfPage = addr-PageAddresses[page];
    AH.lengthTillEndOfPage = pageSize-addr;
    return AH;
}

class pageListHelper
{
public:
    std::vector<unsigned long int> lengthList;
    int startPage;
};

#ifdef RESTRICT_PAGESIZE
unsigned long int virtualMemorySpace::RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len)
{
    addr -= memoryOffset;
    if(islooped)
    {
        addr%=memorySize;
    }
}
#else
#ifdef USE_BYTEWISE_RW
#ifdef EXTRA_DEBUG_MESSAGES
    int printonce = 0;
#endif //EXTRA_DEBUG_MESSAGES
//I absolutely hate that I have to write every byte individually.
//But it's the only way I could make it simple for my smol brain to understand.
//Please someone write maths wizardry to calculate how many bytes have to
// be read/written to each page so it can be done with less calls to writeMem/readMem.
unsigned long int virtualMemorySpace::RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    if(printonce == 0)
    {
        printf("Using byte-wise r/w.\n");
        printonce = 1;
    }
    #endif //EXTRA_DEBUG_MESSAGES
	unsigned long int numBytes = len;

    addr -= memoryOffset;

    if(islooped)
    {
        addr%=memorySize;
    }

	std::vector<addressSpace*> PageList; //indexed list of pointers to pages;
	addressSpace* _search = top;
	if(_search == NULL)
    {
        return 0;
    }
	while(1)
	{
		PageList.push_back(_search);
		//PageAddresses[i]; //indexed list of addresses of pages;
		_search = _search->_next;
		//i++;
		if(_search == NULL)
        {
            break;
        }
	}

	int j=0;
	while(PageList[j]->_size()==0)
    {j++;}
 	int pageEndAddress = PageAddresses[j]+PageList[j]->_size()-1;
	while(pageEndAddress < addr)
	{
		j++;
		if(PageList[j]->_size()==0){continue;}
		pageEndAddress = PageAddresses[j]+PageList[j]->_size()-1;
	}
    int i=0;
	for(i=0; i<len; i++)
	{
		if( ((addr+i)%memorySize) > pageEndAddress)
		{
			j++;
			if(j>=pageCount())
			{
				j=0;
			}
			pageEndAddress = PageAddresses[j]+PageList[j]->_size();
		}
		if(write)
		{
			PageList[j]->writeMem( ((addr+i)%memorySize) ,buf+i,1);
		}
		else
		{
			PageList[j]->readMem( ((addr+i)%memorySize) ,buf+i,1);
		}
	}

	return numBytes;
}
#else

unsigned long int virtualMemorySpace::RW_Mem(bool write,unsigned long int Address, unsigned char* buf, unsigned long int length)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("address:%i\n",(int)Address);
    printf("length:%i\n",(int)length);
    printf("memoryOffset:%i\n",(int)memoryOffset);
    #endif
    Address -= memoryOffset;

    if(islooped)
    {
        Address%=memorySize;
    }

    int totalBytesRead = 0;

    unsigned long int startAddress = Address;
    unsigned long int endAddress = Address+length;
	#ifdef EXTRA_DEBUG_MESSAGES
    printf("startAddress:%i\n",(int)startAddress);
    printf("endAddress:%i\n",(int)endAddress);
	#endif

    while(length>totalBytesRead)
    {
        //printf("startAddress:%i\n",(int)startAddress);
        addressSpace* _search = top;
        int i=0;
        if(startAddress>0)
        {
            while(startAddress>PageAddresses[i]) //finds the first page that the memory region is in.
            {
                i++;
                if(i>PageAddresses.size() || _search->_next == NULL)
                {
                    break;
                }
                _search = _search->_next;
            }
        }
        unsigned long int PageTop = PageAddresses[i]; //top of the memory of the page found
        unsigned long int offset = startAddress-PageTop;
        unsigned long int offset_end = endAddress-PageTop;

		#ifdef EXTRA_DEBUG_MESSAGES
        printf("i:%i\n",(int)i);
        printf("PageTop:%i\n",(int)PageTop);
        printf("offset:%i\n",(int)offset);
        printf("offset_end:%i\n",(int)offset_end);
        printf("_search->_size():%i\n",(int)_search->_size());
		#endif
        if(offset_end+offset <= _search->_size()) //if our data is all in one page.
        {
            if(write == 1)
            {
                _search->writeMem(offset,buf,length-totalBytesRead);
            }
            else
            {
                _search->readMem(offset,buf,length-totalBytesRead);
            }
            return totalBytesRead; // then end here.
        }
        int bytesWritten=0;
        bytesWritten = _search->_size()-offset;
        if(write == 1)
        {
            _search->writeMem(offset,buf,bytesWritten);
        }
        else
        {
            _search->readMem(offset,buf,bytesWritten);
        }
        //Address -= bytesWritten;
        startAddress += bytesWritten;
        endAddress = startAddress+(length-totalBytesRead);
        totalBytesRead += bytesWritten;
    }
    return totalBytesRead;

}

unsigned long int virtualMemorySpace::RW_Mem_From_Page_No(unsigned long int pageNum, bool write, unsigned long int addr, unsigned char* buf, unsigned long int len)
{
    unsigned long int totalBytesRead = 0;

    addressSpace* _search = top;
    unsigned long int i=0;
    while(i < pageNum)
    {
        _search = _search->_next;
        i++;
    }

    if(write)
    {
        _search->writeMem(addr,buf,len);
    }
    else
    {
        _search->readMem(addr,buf,len);
    }
    totalBytesRead = len;

    return totalBytesRead;
}

/*
unsigned long int virtualMemorySpace::RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("write:%i, addr:%i, bufaddr:0x%08x, len:%i\n",write,addr,buf,len);
    //Beep(400,100);
    #endif
    unsigned long int numBytes = len;

    addr -= memoryOffset;

    if(islooped)
    {
        addr%=memorySize;
    }

    bool firstPage = true;

    int i=0;
    addressHelper AH;
    while(1)
    {
        AH = isAddressInPage(addr,i);
        if(AH.isInPage == 1)
        {
            break;
        }
        if(i>=pageCount())
        {
            return 0;
        }
        i++;
    }

    #ifdef EXTRA_DEBUG_MESSAGES
    printf("pagefound:%i\n",i);
    #endif

    while(len > 0)
    {
        unsigned long int _len;
        if(len < PageList[i]->_size())
        {
            _len = len;
        }
        else
        {
            _len = PageList[i]->_size();
        }
        #ifdef EXTRA_DEBUG_MESSAGES
        printf("_len:%i PageSize:%i\n",_len,PageList[i]->_size());
        #endif

        if(firstPage)
        {
            firstPage = false;
            if(write)
            {
                PageList[i]->writeMem(addr-PageAddresses[i],buf+(numBytes-len),_len);
            }
            else
            {
                PageList[i]->readMem(addr-PageAddresses[i],buf+(numBytes-len),_len);
            }
            len -= _len;
        }
        else
        {
            if(write)
            {
                PageList[i]->writeMem(PageAddresses[i],buf+(numBytes-len),_len);
            }
            else
            {
                PageList[i]->readMem(PageAddresses[i],buf+(numBytes-len),_len);
            }
            len -= _len;
        }
        i++;
    }

	return numBytes-len;

}
*/
#endif
#endif //RESTRICT_PAGESIZE

unsigned long int virtualMemorySpace::readMem(unsigned long int Address, unsigned char* buf, unsigned long int length)
{
    return RW_Mem(0, Address, buf, length);
}

unsigned long int virtualMemorySpace::writeMem(unsigned long int Address,unsigned char* buf,unsigned long int length)
{
    return RW_Mem(1, Address, buf, length);
}

unsigned int virtualMemorySpace::pageCount()
{
    if(top == NULL) //if we don't have any pages
    {
        return 0;
    }
    int pagecount = 0;
    addressSpace* _search = top;
    while(_search != NULL) // loop ends when the pointer to _search is set to NULL through the last iterations _search->_next being NULL.
    {
        pagecount += 1;
        _search = _search->_next;
    }
    return pagecount;
}

void virtualMemorySpace::addressRange(unsigned long int* lowMem,unsigned long int* highMem)
{

    //when we return addresses to the outside we add the virtual offset.
    //in other functions that recieve an address we have to subtract the offset.
    //this allows us some memory remapping functionality.

    if(top == NULL) //if we don't have any pages
    {
        *highMem = memoryOffset;
        return;
    }
    *lowMem = memoryOffset;
    unsigned long int byteCount = 0;
    addressSpace* _search = top;
    while(_search != NULL) // loop ends when the pointer to _search is set to NULL through the last iterations _search->_next being NULL.
    {
        byteCount += _search->_size();
        _search = _search->_next;
    }

    *highMem = byteCount+memoryOffset-1; //c++ starts at 0 so the highest address is the number of addresses -1;

    return;
}

