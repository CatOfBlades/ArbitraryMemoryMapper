
#ifndef INTERPROCESSMEMORYSPACE_H_INCLUDED
#define INTERPROCESSMEMORYSPACE_H_INCLUDED

#include "addressSpace.h"
#include <windows.h>

/**
The purpose of this file is to show how one might implement an address space that is in another process
**/

class applicationMem: public addressSpace
{
    public:

    DWORD _processID;
    HANDLE hProcess;
    HANDLE hAddress;

    void attachProcess(DWORD processID);
    void dettachProcess(HANDLE processHandle);

	unsigned long int _pageSize;
	unsigned char* _pageContent;

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();


	applicationMem();
	applicationMem(addressSpace* parent, DWORD procID, unsigned int Size,HANDLE addr);
	~applicationMem();
};


#endif // INTERPROCESSMEMORYSPACE_H_INCLUDED
