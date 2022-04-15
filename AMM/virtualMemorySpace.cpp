
#include "virtualMemorySpace.h"

#ifdef WINBUILD
#include <Windows.h>
#endif // WINBUILD

#include <stdio.h>

virtualMemorySpace::virtualMemorySpace()//if you don't need a memory offset then don't worry about it.
{
    memorySize=0;
    memoryOffset=0;
    islooped = MEMSPACE_LOOP_DEFAULT;
}

virtualMemorySpace::virtualMemorySpace(int memoffset)
{
    memorySize=0;
    memoryOffset = memoffset;
    islooped = MEMSPACE_LOOP_DEFAULT;
}

virtualMemorySpace::~virtualMemorySpace()
{
    while(memorySize > 0)
    {
        removeAddressSpace(0);
    }
}

unsigned int virtualMemorySpace::virtualMemorySpace::pageCountAndRecalcSize()
{
    PageAddresses.clear();

    memorySize = 0;
	unsigned int _count = 0;

	for (std::shared_ptr<addressSpace> x : pageList)
	{
        memorySize += x->_size();
		_count++;
		PageAddresses.push_back(memorySize+memoryOffset);
    }
    return _count;
}

std::shared_ptr<addressSpace> virtualMemorySpace::_getAddressPage(unsigned long int addr,unsigned long int offset) //returns NULL if the address is out of scope of this memory space.
{
    /**
    returns a pointer to the page that the address is in.
    and the offset is how many bytes from the start of the page that the address is at.
    **/

	// will probbably iterate over the 'PageAddresses' vector to solve this one.
}

void virtualMemorySpace::removeAddressSpace(int AS)
{
    if(!pageList.size()) //if we don't have any pages
    {
        return;
    }
    else
    {
		#ifdef DBG_MESSAGES
		printf("removing page:n \n");
		#endif
		pageList.erase(pageList.begin()+AS);
    }
	pageCountAndRecalcSize();
}

void virtualMemorySpace::addAddressSpace(IN std::shared_ptr<addressSpace> AS)
{
	pageList.push_back(AS);
    pageCountAndRecalcSize();
}


bool virtualMemorySpace::isAddressInPage(unsigned long int addr,int page)
{
	//not yet implimented.
    return false;
}


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

	if(!pageList.size())
    {
        return 0;
    }


	int j=0;
	while(pageList.at(j)->_size()==0){j++;} //skip empty pages

 	int pageEndAddress = PageAddresses.at(j)+PageList.at(j)->_size()-1;

	while(pageEndAddress < addr)
	{
		j++;
		if(PageList.at(j)->_size()==0){continue;}
		pageEndAddress = PageAddresses.at(j)+PageList.at(j)->_size()-1;
	}
    int i=0;
	for(i=0; i<len; i++)
	{
		if( addr+i > memorySize ) return 0;
		if( addr+i > pageEndAddress )
		{
			j++;
			if(j>=pageList.size())
			{
				j=0;
				return 0;
			}
			pageEndAddress = PageAddresses.at(j)+PageList.at(j)->_size();
		}
		if(write)
		{
			PageList.at(j)->writeMem( (addr+i) ,buf+i,1);
		}
		else
		{
			PageList.at(j)->readMem( (addr+i) ,buf+i,1);
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
        auto i= 0;
        if(startAddress>0)
        {
            while(startAddress>PageAddresses[i]) //finds the first page that the memory region is in.
            {
                i++;
                if(i>PageAddresses.size())
                {
                    break;
                }
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
        printf("_search->_size():%i\n",pageList.at(i)->_size());
		#endif
        if(offset_end+offset <= pageList.at(i)->_size()) //if our data is all in one page.
        {
            if(write == 1)
            {
                pageList.at(i)->writeMem(offset,buf,length-totalBytesRead);
            }
            else
            {
                pageList.at(i)->readMem(offset,buf,length-totalBytesRead);
            }
            return totalBytesRead; // then end here.
        }
        int bytesWritten=0;
        bytesWritten = pageList.at(i)->_size()-offset;
        if(write == 1)
        {
            pageList.at(i)->writeMem(offset,buf,bytesWritten);
        }
        else
        {
            pageList.at(i)->readMem(offset,buf,bytesWritten);
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

    if(write)
    {
        pageList.at(pageNum)->writeMem(addr,buf,len);
    }
    else
    {
        pageList.at(pageNum)->readMem(addr,buf,len);
    }
    totalBytesRead = len;

    return totalBytesRead;
}

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



