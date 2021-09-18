

#ifndef VIRTUALMEMORYSPACE_H_INCLUDED
#define VIRTUALMEMORYSPACE_H_INCLUDED

#include "../Defines.h"
#include "addressSpace.h"
#include "virtualPage.h"
#include <vector>
using namespace std;

struct addressHelper
{
    bool isInPage;
    unsigned long int offsetFromStartOfPage;
    unsigned long int lengthTillEndOfPage;
};

class virtualMemorySpace
{
    unsigned long int RW_Mem(bool write, unsigned long int addr, unsigned char* buf, unsigned long int len);
    unsigned long int RW_Mem_From_Page_No(unsigned long int pageNum, bool write, unsigned long int addr, unsigned char* buf, unsigned long int len);
    public:
    addressSpace* top;  //The first page of memory in our linked list;
    addressSpace* bottom; //The last page of memory in our linked list;
    unsigned long int memoryOffset; //The virtual offset for addresses in our memory space. (unsigned so no negative offsets)
    unsigned long int memorySize; // calculated as more pages are added refers to how much virtual memory we have addressable in bytes;
    vector<unsigned long int> PageAddresses; //calculated when memorySize is updated.
    vector<addressSpace*> PageList; //calculated when memorySize is updated.
    addressHelper isAddressInPage(unsigned long int addr,int page);//page is the position in the linked list. first page being 0.
    short islooped;
        //'islooped' mods addresses to start back at the beginning if they are past the end of this memory space.
        //set is looped to zero if you want to use it as a proxy for a fragment of memory instead of a whole virtual system.
        //if you don't get it don't worry it's a subtle thing that makes access times faster by ignoring sanity checks when set to zero.
        //IE might crash if you access out of bounds and this isn't equal to one.

    /**
    The memory offset allows for memory spaces to be defined which don't start at 0;
    So if for example the code we are trying to emulate expects the RAM to be between 0x00A00000 and 0x00FF0000
    we can specify that the memory offset is: 0x00A00000
    then address 0x00A39800 would point to address 0x00039800 in our structure.
    **/

	virtualMemorySpace();//if you don't need a memory offset then don't worry about it.

	virtualMemorySpace(int memoffset);

	~virtualMemorySpace();

	void recalcSize();//check the linked list to see how much memory we are working with.

    addressSpace* _getAddressPage(unsigned long int addr,unsigned long int* offset); //returns NULL if the address is out of scope of this memory space.

    void _getPageListInAddressRange( long unsigned int lowAddress, long unsigned int highAddress, addressSpace** pagelist,unsigned int* pagecount);
        //This function intentionally verbose. Sadly it's length could interfere with code written with it.
        //TODO: write wrapper that does not take so many characters to write.
        //pagelist shouldn't be larger then 1024


    //  \/ these functions return the number of bytes successfully written/read. These should always return 'length'
    unsigned long int readMem(unsigned long int Address,unsigned char* buf,unsigned long int length);
    unsigned long int writeMem(unsigned long int Address,unsigned char* buf,unsigned long int length);

    unsigned int pageCount(); //returns the current number of pages in the linked list.
    void addressRange(unsigned long int* lowMem,unsigned long int* highMem);
            //returns the addressable memory across all pages.
            //lowMem is always set to equal to the 'memoryOffset' variable.
            //highMem is set to a pointer to the last addressable byte.

    //void* getAddressPointer(unsigned long int addr); //in an arbitrary memory model, direct address pointers aren't possible, only ones that refer to the virtual memory model.

	void addPage(unsigned int Size);
	void removePage();

	void addAddressSpace(addressSpace* AS);

	int getLastError();

    //void expandTo(unsigned int address, unsigned int Size); //arbitrarily allocate memory from one address to another. Yeah Bad idea.
};




#endif // VIRTUALMEMORYSPACE_H_INCLUDED
