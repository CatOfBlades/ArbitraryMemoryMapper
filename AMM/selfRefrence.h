

#ifndef SELFREFRENCE_H_INCLUDED
#define SELFREFRENCE_H_INCLUDED

#include "addressSpace.h"
#include "virtualMemorySpace.h"

/**
    The idea of this class is to provide an interface to other virtual memory spaces.
    Theoretically one could use it to refer to the host memory space, but if
    memory is accessed in it's domain from it's domain it will infinitely loop.
    Be careful with how you use this.
**/


class memorySpacePage: public addressSpace
{
    public:

	unsigned long int _pageSize;
	union
	{
	    unsigned long int _address;
	    char* _paddress;
	};

	unsigned char* _pageContent;
	virtualMemorySpace* childVM; //the memory space we are reading or writing to through this class.

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	void linkToVM(virtualMemorySpace* VM,unsigned long int address, unsigned int Size);


	memorySpacePage();
	memorySpacePage(virtualMemorySpace* VM,unsigned long int address, unsigned int Size);
	memorySpacePage(virtualMemorySpace* VM,char* address, unsigned int Size);
            //address is the offset from 0 in the virtual memory space.
	memorySpacePage(addressSpace* parent, unsigned int Size);
	~memorySpacePage();
};



#endif // SELFREFRENCE_H_INCLUDED
