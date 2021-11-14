
#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include "config.h"

/*
#ifdef _WIN32
#define WINBUILD
#endif
#ifdef _WIN64
#define WINBUILD
#endif
*/

#ifndef WINBUILD
#define Sleep(x) usleep(x)
WINBASEAPI WINBOOL WINAPI ReadProcessMemory (HANDLE hProcess, char* lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead);
//WINBASEAPI char* WINAPI VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
#endif

#ifdef DBG_MESSAGES
#ifdef VERBOSE_DBG
#define EXTRA_DEBUG_MESSAGES
#endif
#endif



#endif // DEFINES_H_INCLUDED
