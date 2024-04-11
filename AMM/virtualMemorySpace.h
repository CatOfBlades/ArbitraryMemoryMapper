

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

    void updatePageAddresses();


	virtualMemorySpace();//if you don't need a memory offset then don't worry about it.

	virtualMemorySpace(int memoffset);

	~virtualMemorySpace();

    //  \/ these functions return the number of bytes successfully written/read.
    unsigned long int readMem(unsigned long int Address,OUT unsigned char* buf,unsigned long int length);
    unsigned long int writeMem(unsigned long int Address,IN unsigned char* buf,unsigned long int length);
    unsigned long int RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len);

	void addAddressSpace(IN std::shared_ptr<addressSpace> AS);
	void removeAddressSpace(int AS);
};



#endif // VIRTUALMEMORYSPACE_H_INCLUDED
