

#ifndef VIRTUALMEMORYSPACE_H_INCLUDED
#define VIRTUALMEMORYSPACE_H_INCLUDED

#include "../Defines.h"
#include "addressSpace.h"
#include "virtualPage.h"
#include <vector>
#include <memory>

//      \/ these are just to clarify function parameters.
#ifndef OUT
#define OUT
#endif
#ifndef IN
#define IN
#endif

class virtualMemorySpace //MainPageRegistry
{
    public:

	std::vector<std::shared_ptr<addressSpace>> pageList;

    unsigned long int memoryOffset; //The virtual offset for addresses in our memory space. (unsigned so no negative offsets)
    unsigned long int memorySize; // calculated as more pages are added refers to how much virtual memory we have addressable in bytes;
    short islooped;

	std::vector<unsigned long int> PageAddresses; //calculated when memorySize is updated.
    bool isAddressInPage(unsigned long int addr,int page);


	virtualMemorySpace();//if you don't need a memory offset then don't worry about it.

	virtualMemorySpace(int memoffset);

	~virtualMemorySpace();

    std::shared_ptr<addressSpace> _getAddressPage(unsigned long int addr,unsigned long int offset); //returns NULL if the address is out of scope of this memory space.
    unsigned int pageCountAndRecalcSize();
		//returns the current number of pages in the linked list.
		//and recalculates the number of avalible bytes in the memory space and assigns it to the 'memorySize' variable
		//also updates the 'PageAddresses' vector.

    //  \/ these functions return the number of bytes successfully written/read.
    unsigned long int readMem(unsigned long int Address,OUT unsigned char* buf,unsigned long int length);
    unsigned long int writeMem(unsigned long int Address,IN unsigned char* buf,unsigned long int length);
    unsigned long int RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len);
    unsigned long int RW_Mem_From_Page_No(unsigned long int pageNum, bool write, unsigned long int addr, unsigned char* buf, unsigned long int len);

	void addAddressSpace(IN std::shared_ptr<addressSpace> AS);
	void removeAddressSpace(int AS);
};



#endif // VIRTUALMEMORYSPACE_H_INCLUDED
