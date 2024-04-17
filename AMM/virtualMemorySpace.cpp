
#include "virtualMemorySpace.h"

#ifdef WINBUILD
#include <Windows.h>
#endif // WINBUILD

#include <stdio.h>

using std::min;

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
	updatePageAddresses();
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

unsigned long int virtualMemorySpace::RW_Mem(bool write, unsigned long int Address, unsigned char* buf, unsigned long int length)
{

    #ifdef EXTRA_DEBUG_MESSAGES
    printf("address:%i\n",(int)Address);
    printf("length:%i\n",(int)length);
    printf("memoryOffset:%i\n",(int)memoryOffset);
    printf("buf:%p\n",buf);
    #endif //EXTRA_DEBUG_MESSAGES

    Address -= memoryOffset;

    if (islooped) {
        Address %= memorySize;
    }

    unsigned long int totalBytesProcessed = 0;

    while (totalBytesProcessed < length) {
        unsigned long int pageIndex = findPageIndex(Address);

        if (pageIndex < pageList.size()) {
            unsigned long int pageOffset = Address - PageAddresses[pageIndex];

            unsigned long int remainingLength = length - totalBytesProcessed;
            unsigned long int bytesToProcess = min(remainingLength, pageList[pageIndex]->_size() - pageOffset);

            #ifdef EXTRA_DEBUG_MESSAGES
            if (bytesToProcess) {
                printf("page overflow on read/write.\n");
            }
            #endif
            if (write) {
                pageList[pageIndex]->writeMem(pageOffset, buf + totalBytesProcessed, bytesToProcess);
            } else {
                pageList[pageIndex]->readMem(pageOffset, buf + totalBytesProcessed, bytesToProcess);
            }

            totalBytesProcessed += bytesToProcess;
            Address += bytesToProcess;
        } else {
            #ifdef EXTRA_DEBUG_MESSAGES
            printf("Page not found\n");
            #endif
            break; // Page not found, exit loop
        }
    }

    return totalBytesProcessed;
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

void virtualMemorySpace::updatePageAddresses()
{
    PageAddresses.clear(); // Clear the existing PageAddresses

    unsigned long int address = 0; // Start address for the first page

    // Iterate over each page in pageList
    for (const auto& page : pageList)
    {
        // Add the address of the current page to PageAddresses
        PageAddresses.push_back(address);

        // Increment the address by the size of the current page
        address += page->_size();
    }
}

unsigned long int virtualMemorySpace::findPageIndex(unsigned long int address)
{
    // Binary search through PageAddresses to find the index of the page containing the given address
    unsigned long int left = 0;
    unsigned long int right = PageAddresses.size() - 1;

    while (left <= right) {
        unsigned long int mid = left + (right - left) / 2;

        if (PageAddresses[mid] <= address && (mid == PageAddresses.size() - 1 || PageAddresses[mid + 1] > address)) {
            // Found the page containing the address
            return mid;
        } else if (PageAddresses[mid] > address) {
            // Search in the left half
            right = mid - 1;
        } else {
            // Search in the right half
            left = mid + 1;
        }
    }

    // If the address is beyond the last page, return the index of the last page
    return PageAddresses.size() - 1;
}




