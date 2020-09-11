
#include "interprocessMemorySpace.h"

/**
The purpose of this file is to show how one might implement an address space that is in another process
**/


void applicationMem::attachProcess(DWORD processID)
{
    hProcess = nullptr;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,processID);
    _processID = processID;
}
void applicationMem::dettachProcess(HANDLE processHandle)
{
    CloseHandle(hProcess);
    hProcess = nullptr;
    _processID = 0;
}
unsigned long int applicationMem::_size()
{
    return _pageSize;
}
bool applicationMem::_is_free()
{
    if(hProcess == nullptr)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
unsigned char* applicationMem::_content()
{
    if(hProcess == nullptr)
    {
        return 0;
    }
    bool success = ReadProcessMemory(hProcess,(void*)hAddress,_pageContent,_pageSize,0);
    if(success)
    {
        return _pageContent;
    }
    return nullptr;
}
unsigned char applicationMem::readByte(unsigned long int offset)
{
    char out;
    bool success = ReadProcessMemory(hProcess,(void*)(hAddress+offset),&out,1,0);
    if(success)
    {
        return out;
    }
    else
    {
        return 0;
    }
}
void applicationMem::writeByte(unsigned long int offset,unsigned char Byt)
{
    WriteProcessMemory(hProcess,(void*)hAddress+offset,&Byt,1,0);
}

void applicationMem::readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len)
{
    ReadProcessMemory(hProcess,(void*)hAddress+offset,buffer,len,0);
}
void applicationMem::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    //Beep(400,100);
    //printf("ApplicationMem write\n\taddress:%X\n\toffset:%i\n\thProcess:%x\n\tlength:%i\n",(void*)hAddress,(int)offset,hProcess,(int)len);
    SIZE_T written = 0;

    DWORD oldProtect = 0;
    VirtualProtectEx( hProcess, (void*)hAddress+offset, 256, PAGE_EXECUTE_READWRITE, &oldProtect );
    WriteProcessMemory(hProcess,(void*)hAddress+offset,(void*)Byt,(SIZE_T)len,&written);
    VirtualProtectEx( hProcess, (void*)hAddress+offset, 256, oldProtect, NULL ); //restore the origina
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
applicationMem::applicationMem(addressSpace* parent, DWORD procID, unsigned int Size,HANDLE addr)
{
    //Beep(400,100);
    memoryTypeID = "Appl_Mem";
    _pageSize = Size;
    attachProcess(procID);
    _pageContent = new unsigned char[_pageSize];
    hAddress = addr;

    _last = parent;
    _this = this;
    if(parent)
    {
        _next = parent->_next;
        if(_next == nullptr)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = parent->_top;
            while(_search != nullptr)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = parent->_bottom;
        }
        parent->_next = _this;
        _top = parent->_top;
    }
    else
    {
        _next = nullptr;
        _bottom = nullptr;
        _top = nullptr;
    }
}
applicationMem::~applicationMem()
{
    dettachProcess(hProcess);
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
}

