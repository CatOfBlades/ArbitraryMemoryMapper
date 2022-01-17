
#include "WindowsPageAccessor.h"
#include "../Defines.h"
#include "psapi.h"

systemDataManager::systemDataManager()
{
    thisProcessId = GetCurrentProcessId();
    thisProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,0,thisProcessId);

    if(thisProcessHandle == NULL)
    {
        lastError = GetLastError();
        #ifdef DBG_MESSAGES
        printf( "failed to open this process, Last Error Code: %i", lastError);
        #endif
        return;
    }
    GetNativeSystemInfo(&SysInf);
    p_lowAddr = SysInf.lpMinimumApplicationAddress;
    p_highAddr = SysInf.lpMaximumApplicationAddress;
    PageSize = SysInf.dwPageSize;

    if(!GetProcessWorkingSetSize(thisProcessHandle,&minWorkingSetSize,&maxWorkingSetSize))
    {
        #ifdef DBG_MESSAGES
        lastError = GetLastError();
        //printf( "Could not get process working set size. Error: %i \n", lastError);
        #endif
    }
    else
    {
        if(minWorkingSetSize != PROGRAM_MIN_WORKING_SET_SIZE)
        {
            minWorkingSetSize = PROGRAM_MIN_WORKING_SET_SIZE;
        }
        if(maxWorkingSetSize != PROGRAM_MAX_WORKING_SET_SIZE)
        {
            maxWorkingSetSize = PROGRAM_MAX_WORKING_SET_SIZE;
        }
        if(!SetProcessWorkingSetSize(thisProcessHandle,minWorkingSetSize,maxWorkingSetSize))
        {
            #ifdef DBG_MESSAGES
            lastError = GetLastError();
            //printf( "Could not set process working set size. Error: %i \n", lastError);
            #endif
        }
        if(!GetProcessMemoryInfo(thisProcessHandle,&memCounters,sizeof(memCounters)))
        {
            #ifdef DBG_MESSAGES
            lastError = GetLastError();
            //printf( "Could not get process memory info structure. Error: %i \n", lastError);
            #endif
        }
  }

    #ifdef DBG_MESSAGES
    printf( "pagesize is: %i \n", PageSize);
    printf( "The lowest address is: %p \n", p_lowAddr);
    printf( "The highest address is: %p \n", p_highAddr);
    #endif
    return;
}
systemDataManager::~systemDataManager()
{
    return;
}


memoryPageAccessor::memoryPageAccessor(systemDataManager* psdm)
{
    broken = false;
    p_sdm = psdm;
    thisProcId = p_sdm->thisProcessId;
    thisProcHandle = p_sdm->thisProcessHandle;
    memPageSize = p_sdm->PageSize;
    committed=0;
    p_memRegion = static_cast<char*>(VirtualAlloc(0,memPageSize,MEM_RESERVE,PAGE_NOACCESS));
    access = PAGE_NOACCESS;
    if(p_memRegion==NULL)
    {
        broken = true;
        lastErr = GetLastError();
        #ifdef DEBUG_BEEP
        Beep(300,400);
        #endif // DEBUG_BEEP
    }
}
memoryPageAccessor::~memoryPageAccessor()
{
    if(broken)
    {
        return; //we don't know how broken so failsafe.
        //this could result in memory leaks.
    }
    VirtualFree(p_memRegion,0,MEM_RELEASE);
}
bool memoryPageAccessor::changeAccess(DWORD flprotect)
{
    access = flprotect;
    bool retval = VirtualProtect(p_memRegion,memPageSize,flprotect,&_oldAccess);
    if(retval==false)
    {
        broken = true;
        lastErr = GetLastError();
        #ifdef DEBUG_BEEP
        Beep(300,400);
        #endif // DEBUG_BEEP
    }
    return retval;
}
bool memoryPageAccessor::committ()
{
    p_memRegion = static_cast<char*>(VirtualAlloc(p_memRegion,memPageSize,MEM_COMMIT,access));
    if(p_memRegion==NULL)
    {
        broken = true;
        lastErr = GetLastError();
        #ifdef DEBUG_BEEP
        Beep(300,400);
        #endif // DEBUG_BEEP
        return false;
    }
    return true;
}




unsigned long int MPA_page::_size()
{
    return (unsigned long int)MPA->memPageSize;
}
bool MPA_page::_is_free()
{
    return MPA->committed&&!MPA->broken;
}
unsigned char* MPA_page::_content()
{
    return (unsigned char*)MPA->p_memRegion;
}

unsigned char MPA_page::readByte(unsigned long int offset)
{
    unsigned char retval;
    ReadProcessMemory(MPA->thisProcHandle,(MPA->p_memRegion+offset),&retval,1,NULL);
    return retval;
}
void MPA_page::writeByte(unsigned long int offset,unsigned char Byt)
{
    WriteProcessMemory(MPA->thisProcHandle,MPA->p_memRegion+offset,&Byt,1,NULL);
}

void MPA_page::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
    ReadProcessMemory(MPA->thisProcHandle,MPA->p_memRegion+offset,buffer,len,NULL);
}
void MPA_page::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    WriteProcessMemory(MPA->thisProcHandle,MPA->p_memRegion+offset,Byt,len,NULL);
}

void MPA_page::cleanupAndUnlink()
{
    return;
}
void MPA_page::setParent(addressSpace* addrSp)
{
    _last = addrSp;
    if(addrSp)
    {
        _next = addrSp->_next;
        if(_next == NULL)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = addrSp->_top;
            while(_search != NULL)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = addrSp->_bottom;
        }
        addrSp->_next = _this;
        _top = addrSp->_top;
    }
    else
    {
        _next = NULL;
        _bottom = NULL;
        _top = NULL;
    }
}

MPA_page::MPA_page(memoryPageAccessor* _mpa)
{
    MPA = _mpa;
    selfConstructed = 0;
    selfConstructedDM = 0;
}
MPA_page::MPA_page()
{
    systemDataManager* SDM = new systemDataManager();
    MPA = new memoryPageAccessor(SDM);
    selfConstructed = 1;
    selfConstructedDM = 1;
}
MPA_page::MPA_page(systemDataManager* psdm)
{
    MPA = new memoryPageAccessor(psdm);
    selfConstructed = 1;
    selfConstructedDM = 0;
}

MPA_page::~MPA_page()
{
    if(selfConstructedDM)
    {
        delete(MPA->p_sdm);
    }
    if(selfConstructed)
    {
        delete(MPA);
    }
}

DWORD WINAPI RWX_Thread( LPVOID lpParam );

int makeRWX()
{
    CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size
                RWX_Thread,       // thread function name
                0,//pDataArray[i],          // argument to thread function
                0,                      // use default creation flags
                0);//&dwThreadIdArray[i]);   // returns the thread identifier
    return 0;
}
DWORD WINAPI RWX_Thread( LPVOID lpParam )
{
    bool exitThread = false;
    SYSTEM_INFO SysInf;
    DWORD PageSize;
    char* p_lowAddr;
    char* p_highAddr;
    char* p_memRegion;
    DWORD oldAccess;
    DWORD thisProcessId = GetCurrentProcessId();
    HANDLE thisProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,0,thisProcessId);
    if(thisProcessHandle == NULL)
    {
        exitThread = true;
    }
    GetNativeSystemInfo(&SysInf);
    p_lowAddr = static_cast<char*>(SysInf.lpMinimumApplicationAddress);
    p_highAddr = static_cast<char*>(SysInf.lpMaximumApplicationAddress);
    PageSize = SysInf.dwPageSize;

    p_memRegion = p_lowAddr;
    while(1)
    {
        if(exitThread){break;}
        VirtualProtect(p_memRegion,PageSize,PAGE_EXECUTE_READWRITE,&oldAccess);
        if(p_memRegion < p_highAddr)
        {
            p_memRegion+=PageSize;
        }
        else
        {
            exitThread = true;
        }
    }
    //Beep(400,400);
    return 0;
}

