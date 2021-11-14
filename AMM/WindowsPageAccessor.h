

#ifndef WINDOWSPAGEACCESSOR_H_INCLUDED
#define WINDOWSPAGEACCESSOR_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <vector>
#include "addressSpace.h"
#include "psapi.h"

//Essentially just a more complicated virtual page where the page size is the same as the systems page size.
//useful for windows specific applications.

int makeRWX(); //makes as many pages as we can read write and execute permissions


class memoryPageAccessor;

class systemDataManager
{
    public:
    std::vector<memoryPageAccessor*> memoryPageAccessorList;
    SYSTEM_INFO SysInf;
    DWORD PageSize;
    LPVOID p_lowAddr;
    LPVOID p_highAddr;
    DWORD lastError;
    DWORD thisProcessId;
    HANDLE thisProcessHandle;
    HANDLE thisProcessTokenHandle;
    HANDLE thisProcessDuplicateToken;
    SIZE_T minWorkingSetSize;
    SIZE_T maxWorkingSetSize;
    PROCESS_MEMORY_COUNTERS memCounters;
    systemDataManager();
    ~systemDataManager();
};

class memoryPageAccessor
{
    DWORD _oldAccess;
public:
    systemDataManager* p_sdm;
    DWORD access;
    DWORD memPageSize;
    char* p_memRegion;
    bool broken; //if there was an error in construction this will be true. otherwise false.
    bool committed;
    DWORD thisProcId;
    HANDLE thisProcHandle;
    DWORD lastErr;
    memoryPageAccessor(systemDataManager* psdm);
    ~memoryPageAccessor();
    bool changeAccess(DWORD flprotect);
    bool committ();
};

class MPA_page: public addressSpace
{
    bool selfConstructed;
    bool selfConstructedDM;
public:
    memoryPageAccessor* MPA;
	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();

	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	void setParent(addressSpace* addrSp);
	MPA_page(memoryPageAccessor* _mpa);
	MPA_page();
	MPA_page(systemDataManager* psdm);
	~MPA_page();
};

#endif // WINDOWSPAGEACCESSOR_H_INCLUDED
