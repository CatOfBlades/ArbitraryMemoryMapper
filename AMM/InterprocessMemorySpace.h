
#ifndef INTERPROCESSMEMORYSPACE_H_INCLUDED
#define INTERPROCESSMEMORYSPACE_H_INCLUDED

#include "addressSpace.h"

#include "../Defines.h"
#ifdef __linux__
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#endif

#ifdef WINBUILD
#define _WIN32
#endif //WINBUILD
#ifdef _WIN32
#include <Windows.h>
#include <iostream>
#endif //WIN32


/**
The purpose of this file is to show how one might implement an address space that is in another process
**/

class applicationMem: public addressSpace
{
    public:
    void setProcessID(DWORD processID);

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
	applicationMem( DWORD procID, unsigned int Size,char* addr);
	~applicationMem();
	private:
    DWORD _processID;
    char* hAddress;
};


#endif // INTERPROCESSMEMORYSPACE_H_INCLUDED
