
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

/*
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
*/

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
	//pageCountAndRecalcSize();
}

void virtualMemorySpace::addAddressSpace(IN std::shared_ptr<addressSpace> AS)
{
	pageList.push_back(AS);
    //pageCountAndRecalcSize();
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
                if(i>=PageAddresses.size())
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

#endif //RESTRICT_PAGESIZE

unsigned long int virtualMemorySpace::readMem(unsigned long int Address, unsigned char* buf, unsigned long int length)
{
    return RW_Mem(0, Address, buf, length);
}

unsigned long int virtualMemorySpace::writeMem(unsigned long int Address,unsigned char* buf,unsigned long int length)
{
    return RW_Mem(1, Address, buf, length);
}



