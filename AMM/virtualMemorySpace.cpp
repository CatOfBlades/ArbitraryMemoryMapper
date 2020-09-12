
#include "virtualMemorySpace.h"
#include <windows.h>
#include <stdio.h>

virtualMemorySpace::virtualMemorySpace()//if you don't need a memory offset then don't worry about it.
{
    memoryOffset=0;
    top=0;
}

virtualMemorySpace::virtualMemorySpace(int memoffset)
{
    memoryOffset = memoffset;
    top=0;
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

    memorySize = 0;
    if(top == NULL){return;};
    addressSpace* _search = top;
    while(_search != NULL)// loop ends when the pointer to _search is set to zero through the last iterations _search->_next being zero.
    {
        PageAddresses.push_back(memorySize);
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


    if(addr > memorySize){return NULL;};//address is outside of the scope of this memory space;

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

void virtualMemorySpace::addPage(unsigned int Size)
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
            //printf("removing page:n \n");
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
    //long long int addressDifference = highAddress - lowAddress; //how many bytes to go through.

    //printf("addressDif:%i\n",(int)addressDifference); //for checking our work so far

    pagelist[0] = NULL; //has to be something that is not a valid pointer.

    while(lowAddress<=highAddress)
    {
        tmp_AS = _getAddressPage(lowAddress, &tmp_PageOffset);

        //printf("tmp_AS_Size:%i\n",(int)tmp_AS->_size());
        //printf("tmpPageOffs:%i\n",(int)tmp_PageOffset);
        //printf("pageCount:%i\n",(int)*pagecount);

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

unsigned long int virtualMemorySpace::readMem(unsigned long int Address, unsigned char* buf, unsigned long int length)
{

    //printf("address:%i\n",(int)Address);
    //printf("length:%i\n",(int)length);
    //printf("memoryOffset:%i\n",(int)memoryOffset);
    Address -= memoryOffset;
    int totalBytesRead = 0;

    unsigned long int startAddress = Address;
    unsigned long int endAddress = Address+length;
    //printf("startAddress:%i\n",(int)startAddress);
    //printf("endAddress:%i\n",(int)endAddress);

    while(length>totalBytesRead)
    {
        //Beep(400,100);
        PageAddresses;

        addressSpace* _search = top;
        int i=0;
        if(startAddress>0)
        {
            while(startAddress>=PageAddresses[i])
            {
                i++;
                if(i>PageAddresses.size() || _search->_next == NULL)
                {
                    break;
                    //return;
                }
                _search = _search->_next;
            }
        }
        unsigned long int PageTop = PageAddresses[i]; //top of the memory of the page found
        unsigned long int offset = startAddress-PageTop;
        unsigned long int offset_end = endAddress-PageTop;

        //Beep(400,100);
        //printf("i:%i\n",(int)i);
        //printf("PageTop:%i\n",(int)PageTop);
        //printf("offset:%i\n",(int)offset);
        //printf("offset_end:%i\n",(int)offset_end);
        //printf("_search->_size():%i\n",(int)_search->_size());
        if(offset_end+offset <= _search->_size()) //if our data is all in one page.
        {
            //Beep(400,100);
            _search->readMem(offset,buf,length-totalBytesRead);
            return totalBytesRead;//_search->_size()-offset;
            //break;
        }
        int bytesWritten=0;
        bytesWritten = _search->_size()-offset;
        _search->readMem(offset,buf,bytesWritten);
        Address -= bytesWritten;
        totalBytesRead += bytesWritten;
    }
    //Beep(400,100);
    return totalBytesRead;

    /** Old Buggy Code
    //printf("Address:%i,length:%i\n",Address,length);
    unsigned long int lowAddress = Address;
    unsigned long int highAddress = Address+length;
    lowAddress -= memoryOffset;  //when we receive addresses from the outside we subtract the virtual offset.
    highAddress -= memoryOffset; //in other functions that return an address we have to add that offset back.
                                 //this allows us some memory remapping functionality.

    if(top == NULL) //if we don't have any pages
    {
        //Beep(400,100);
        return 0; //no bytes can be written if theres no space to write to.
    }

    unsigned long int tmp_PageOffset = 0;
    addressSpace* tmp_AS;
    while(lowAddress<=highAddress)
    {
        tmp_AS = _getAddressPage(lowAddress, &tmp_PageOffset);
        if(highAddress-lowAddress < tmp_AS->_size()-tmp_PageOffset) //if the remaining data length is less then the size of the page.
        {
            //printf("maybe? \n");
            //printf("low address:%i\n",(int)lowAddress);
            //printf("tmp_PageOffset:%i\n",(int)tmp_PageOffset);
            //printf("bytes remain:%i\n",(int)lowAddress-Address);
            //printf("highAddress-lowAddress:%i\n",(int)highAddress-lowAddress);
            tmp_AS->readMem(tmp_PageOffset,buf+lowAddress-Address,highAddress-lowAddress);
                //lowAddress-Address is the number of bytes read so far.
                //highAddress-lowAddress is how much data we have left to read.
                //so this writes whats left from the remaining spot in the buffer.
            lowAddress += tmp_AS->_size()-tmp_PageOffset;
            return lowAddress-Address;
        }
        else
        {
            //printf("did it work? \n");
            tmp_AS->readMem(lowAddress,buf+lowAddress-Address,tmp_AS->_size()-tmp_PageOffset); //read what you can till the end of the page
                //the rest of the page is the size of the page minus the offset within the page that our address is at.
            lowAddress += tmp_AS->_size()-tmp_PageOffset; //skip this page
        }
        //lowAddress += 1; //go to the next byte.
    }
    return lowAddress-Address;
    **/
}

unsigned long int virtualMemorySpace::writeMem(unsigned long int Address,unsigned char* buf,unsigned long int length)
{

    //printf("address:%i\n",(int)Address);
    //printf("length:%i\n",(int)length);
    //printf("memoryOffset:%i\n",(int)memoryOffset);
    Address -= memoryOffset;
    int totalBytesWritten = 0;

    unsigned long int startAddress = Address;
    unsigned long int endAddress = Address+length;
    //printf("startAddress:%i\n",(int)startAddress);
    //printf("endAddress:%i\n",(int)endAddress);

    while(length>totalBytesWritten)
    {
        //Beep(400,100);
        PageAddresses;

        addressSpace* _search = top;
        int i=0;
        if(startAddress>0)
        {
            while(startAddress>=PageAddresses[i])
            {
                i++;
                if(i>PageAddresses.size() || _search->_next == NULL)
                {
                    break;
                    //return;
                }
                _search = _search->_next;
            }
        }
        unsigned long int PageTop = PageAddresses[i]; //top of the memory of the page found
        unsigned long int offset = startAddress-PageTop;
        unsigned long int offset_end = endAddress-PageTop;

        //Beep(400,100);
        //printf("i:%i\n",(int)i);
        //printf("PageTop:%i\n",(int)PageTop);
        //printf("offset:%i\n",(int)offset);
        //printf("offset_end:%i\n",(int)offset_end);
        //printf("_search->_size():%i\n",(int)_search->_size());
        if(offset_end+offset <= _search->_size()) //if our data is all in one page.
        {
            //Beep(400,100);
            _search->writeMem(offset,buf,length-totalBytesWritten);
            return totalBytesWritten;//_search->_size()-offset;
            //break;
        }
        int bytesWritten=0;
        bytesWritten = _search->_size()-offset;
        _search->writeMem(offset,buf,bytesWritten);
        Address -= bytesWritten;
        totalBytesWritten += bytesWritten;
    }
    //Beep(400,100);
    return totalBytesWritten;

    /** Old Buggy Code
    unsigned long int lowAddress = Address;
    unsigned long int highAddress = Address+length;
    lowAddress -= memoryOffset;  //when we receive addresses from the outside we subtract the virtual offset.
    highAddress -= memoryOffset; //in other functions that return an address we have to add that offset back.
                                 //this allows us some memory remapping functionality.
    //printf("maybe? \n");
    //printf("address:%i\n",(int)Address);
    //printf("length:%i\n",(int)length);

    if(top == NULL) //if we don't have any pages
    {
        return 0; //no bytes can be written if theres no space to write to.
    }

    unsigned long int tmp_PageOffset = 0;
    addressSpace* tmp_AS;
    while(lowAddress<=highAddress)
    {
        tmp_AS = _getAddressPage(lowAddress, &tmp_PageOffset);
        if(highAddress-lowAddress < tmp_AS->_size()-tmp_PageOffset) //if the remaining data length is less then the size of the page.
        {
            tmp_AS->writeMem(tmp_PageOffset,buf+lowAddress-Address,highAddress-lowAddress);
                //lowAddress-Address is the number of bytes written so far.
                //highAddress-lowAddress is how much data we have left to write.
                //so this writes whats left from the remaining spot in the buffer.
            lowAddress += tmp_AS->_size()-tmp_PageOffset;
            return lowAddress-Address;
        }
        else
        {
            tmp_AS->writeMem(lowAddress,buf+lowAddress-Address,tmp_AS->_size()-tmp_PageOffset); //write what you can till the end of the page
                //the rest of the page is the size of the page minus the offset within the page that our address is at.
            lowAddress += tmp_AS->_size()-tmp_PageOffset; //skip this page
        }
        //lowAddress += 1; //go to the next byte.
    }
    return lowAddress-Address;

    **/
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

