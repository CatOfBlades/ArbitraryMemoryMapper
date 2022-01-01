
#include "../Defines.h"

#ifdef WINBUILD //currently interprocess is only supported in windows

#include "InterprocessMemorySpace.h"

#include <stdio.h>
using namespace std;

/**
The purpose of this file is to show how one might implement an address space that is in another process
**/


void applicationMem::attachProcess(DWORD processID)
{
    hProcess = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,processID);
    _processID = processID;
}
void applicationMem::dettachProcess(HANDLE processHandle)
{
    CloseHandle(hProcess);
    hProcess = NULL;
    _processID = 0;
}
unsigned long int applicationMem::_size()
{
    return _pageSize;
}
bool applicationMem::_is_free()
{
    if(hProcess == NULL)
    {
        return 0; // h_process not loaded, no size.
    }
    else
    {
        return 1;
    }
}
unsigned char* applicationMem::_content()
{
    if(hProcess == NULL)
    {
        return 0; // h_process not loaded, no content.
    }
    bool success = ReadProcessMemory(hProcess,hAddress,_pageContent,_pageSize,0);
    if(success)
    {
        return _pageContent;
    }
    return NULL;
}
unsigned char applicationMem::readByte(unsigned long int offset)
{
    char out;
    char* addr = hAddress+offset;
    bool success = ReadProcessMemory(hProcess,addr,&out,1,0);
    if(success)
    {
        return out;
    }
    else
    {
        return 0; //couldn't read process memory, failback to reading zeros.
    }
}
void applicationMem::writeByte(unsigned long int offset,unsigned char Byt)
{
    WriteProcessMemory(hProcess,hAddress+offset,&Byt,1,0);
}

void applicationMem::readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len)
{
    ReadProcessMemory(hProcess,hAddress+offset,buffer,len,0);
}
void applicationMem::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    //Beep(400,100);
    //printf("ApplicationMem write\n\taddress:%X\n\toffset:%i\n\thProcess:%x\n\tlength:%i\n",hAddress,(int)offset,hProcess,(int)len);
    SIZE_T written = 0;

    DWORD oldProtect = 0;
    VirtualProtectEx( hProcess, hAddress+offset, 256, PAGE_EXECUTE_READWRITE, &oldProtect );
    WriteProcessMemory(hProcess,hAddress+offset,Byt,(SIZE_T)len,&written);
    VirtualProtectEx( hProcess, hAddress+offset, 256, oldProtect, NULL ); //restore the origina
    //printf("\tbytes Written:%i\n",written);
    if(written==0)
    {
        int errcode = GetLastError();
        //printf("\tzero Written, last error code:%i\n",errcode);
        printf("InterprocessPage->writeProcessMemory encountered error code:%i\n",errcode);
        printf("\tNo bytes actually written, but continuing as if they were.\n");
    }
}

void applicationMem::cleanupAndUnlink()
{
    //dettachProcess(hProcess);
    //delete [] _pageContent; //allready deleted in the destructor
    delete this; // only used because the 'virtualMemorySpace' object calls 'new' to create these objects
        //so don't call 'cleanupAndUnlink()' if created in another way.
}

applicationMem::applicationMem()
{
    memoryTypeID = "Appl_Mem";
    _pageSize = 0;
}
applicationMem::applicationMem(addressSpace* parent, DWORD procID, unsigned int Size,char* addr)
{
    //Beep(400,100);
    memoryTypeID = "Appl_Mem";
    _pageSize = Size;
    attachProcess(procID);
    _pageContent = new unsigned char[_pageSize];
    hAddress = addr;

    initialLink(parent, Size);
}
applicationMem::~applicationMem()
{
    dettachProcess(hProcess);
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
}

#endif //WINBUILD
