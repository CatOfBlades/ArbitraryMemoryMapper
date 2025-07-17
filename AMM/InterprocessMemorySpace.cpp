
#include <cstdint>

#include "../Defines.h"

#include "InterprocessMemorySpace.h"

//doing some hacky stuff to make memory access look like windows accesses in the code base.
#ifdef __linux__
#include "../LinuxMemory/src/LinuxMemoryAPI/LinuxMemory.h"
#define HANDLE LinuxProc_t
#ifndef WINBOOL
#define WINBOOL uint
#endif // WINBOOL
#ifndef DWORD
#define DWORD uint
#endif // DWORD
#ifndef SIZE_T
#define SIZE_T size_t
#endif // SIZE_T

HANDLE OpenProcess (DWORD dwDesiredAccess, WINBOOL bInheritHandle, DWORD dwProcessId)
{
    LinuxProc_t ProcessHandle;
    ProcessHandle.ProcessName = "";
    ProcessHandle.ProcessID = (pid_t)dwProcessId;
    ProcessHandle.ProcessBaseAddress = 0;

    return ProcessHandle;
}

#endif // __linux__

#define processRead 1
#define processWrite 0

const int word_size = sizeof(void*);

// Read the memory of the given process
bool accessMemory(unsigned int pid, uint64_t address, void* buffer, size_t datalen, bool isRead) {

    // Open the target process
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
    if (hProcess == NULL) {
        #ifdef WINBUILD
        OutputDebugStringA("Error: Failed to open process \n");
        #endif // WINBUILD
        std::cerr << "Error: Failed to open process " << pid << std::endl;
        return false;
    }

    #ifdef __linux__
    // Check if the address and datalen are aligned to a word boundary
    uint64_t newaddress;
    size_t newdatalen;
    uint64_t addressdif = 0;
    size_t word_size = sizeof(void*);
    if (address % word_size != 0 || datalen % word_size != 0) {
        // Adjust the address and datalen to be aligned to a word boundary
        newaddress = address - (address % word_size);
        newdatalen = datalen + (address % word_size);
        addressdif = address-newaddress;
    }
    else
    {
        newaddress = address;
        newdatalen = datalen;
        addressdif = 0;
    }
    #endif


    //uint8_t _buffer[max_variable_size];

    if(isRead)
    {

        SIZE_T bytesRead;
        if (!ReadProcessMemory(hProcess, (void*)address, buffer, datalen, &bytesRead)) {
            #ifdef WINBUILD
            OutputDebugStringA("Error: Failed to read memory for process \n");
            char errorcode[255];
            sprintf(errorcode,"Last Error: %i\n",(int)GetLastError());
            OutputDebugStringA(errorcode);
            #endif // WINBUILD
            std::cerr << "Error: Failed to read memory for process " << pid << std::endl;
            return false;
        }

        // Check if the read was partial
        if (bytesRead < datalen) {
            #ifdef WINBUILD
            OutputDebugStringA("Error: Partial read from process \n");
            #endif // WINBUILD
            std::cerr << "Error: Partial read from process " << pid << std::endl;
            return false;
        }
    }
    else //isRead == 0 AKA write
    {
        // Write the value of the memory to the address
        SIZE_T bytesWritten;
        if (!WriteProcessMemory(hProcess,(void*)address,buffer,datalen,&bytesWritten))
        {
            #ifdef WINBUILD
            OutputDebugStringA("Error: Failed to write memory to process \n");
            char errorcode[255];
            sprintf(errorcode,"Last Error: %i\n",(int)GetLastError());
            OutputDebugStringA(errorcode);
            #endif // WINBUILD
            std::cerr << "Error: Failed to write memory to process " << pid << std::endl;
        }
        if (bytesWritten < datalen) {
            #ifdef WINBUILD
            OutputDebugStringA("Error: Partial write to process \n");
            #endif // WINBUILD
            std::cerr << "Error: Partial write to process " << pid << std::endl;
            return false;
        }
    }


    // Detach from the process
    #ifdef _WIN32
    if(!CloseHandle(hProcess)) {
        #ifdef WINBUILD
        OutputDebugStringA("Error: Failed to detach from process \n");
        #endif // WINBUILD
        std::cerr << "Error: Failed to detach from process " << pid << std::endl;
        return false;
    }
    #endif //_WIN32

    return true;
}


using namespace std;

/**
The purpose of this file is to show how one might implement an address space that is in another process
**/

void applicationMem::setProcessID(DWORD processID)
{
    _processID = processID;
}

unsigned long int applicationMem::_size()
{
    return _pageSize;
}
bool applicationMem::_is_free()
{
    return 1;
}
unsigned char* applicationMem::_content()
{
    return NULL;
}
unsigned char applicationMem::readByte(unsigned long int offset)
{
    char out;
    char* addr = hAddress+offset;
    bool success = accessMemory(_processID, (uint64_t)addr, &out, 1, 1);
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
    accessMemory(_processID, (uint64_t)hAddress+offset, &Byt, 1, 0);
}

void applicationMem::readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len)
{
    accessMemory(_processID, (uint64_t)hAddress+offset, buffer, 1, 1);
}
void applicationMem::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    //Beep(400,100);
    #ifdef EXTRA_DEBUG_MESSAGES
    OutputDebugStringA("applicationmem writemem function called.");// offset:%i length:%i\n",offset,len);
    #endif //EXTRA_DEBUG_MESSAGES
    //printf("ApplicationMem write\n\taddress:%X\n\toffset:%i\n\thProcess:%x\n\tlength:%i\n",hAddress,(int)offset,hProcess,(int)len);
    //SIZE_T written = 0;
	int written = 0;

    #ifdef _WIN32
    DWORD oldProtect = 0;
    //VirtualProtectEx( hProcess, hAddress+offset, 256, PAGE_EXECUTE_READWRITE, &oldProtect );
    //WriteProcessMemory(hProcess,hAddress+offset,Byt,(SIZE_T)len,&written);
    accessMemory(_processID, (uint64_t)hAddress+offset, Byt,(SIZE_T)len, 0);
    //VirtualProtectEx( hProcess, hAddress+offset, 256, oldProtect, NULL ); //restore the origina
    #endif //_WIN32
    //printf("\tbytes Written:%i\n",written);
    if(written==0)
    {
		#ifndef __linux__
        int errcode = GetLastError();
        //printf("\tzero Written, last error code:%i\n",errcode);
        printf("InterprocessPage->writeProcessMemory encountered error code:%i\n",errcode);
		#endif
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
applicationMem::applicationMem( DWORD procID, unsigned int Size,char* addr)
{
    //Beep(400,100);
    #ifdef EXTRA_DEBUG_MESSAGES
    OutputDebugStringA("applicationmem constructer called\n");
    #endif //EXTRA_DEBUG_MESSAGES
    memoryTypeID = "Appl_Mem";
    _pageSize = Size;
    _processID = procID;
    _pageContent = new unsigned char[_pageSize];
    hAddress = addr;

}
applicationMem::~applicationMem()
{
    //dettachProcess(hProcess);
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
}
