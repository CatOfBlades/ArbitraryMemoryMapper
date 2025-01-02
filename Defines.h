
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

/*
#ifdef __cplusplus
namespace alfaFlags
{
const static int
        a = 0b00000000,
        b = 0b00000001,
        c = 0b00000010,
        d = 0b00000100,
        e = 0b00001000,
        f = 0b00010000,
        g = 0b00100000,
        h = 0b01000000,
        i = 0b10000000;
}
#endif // __cplusplus
*/

#ifdef LUA51
#define lua_getglobal(x,y) lua_pushstring(x, y); lua_gettable(x, LUA_REGISTRYINDEX);
#define lua_setglobal(x,y) lua_pushstring(x, y); lua_settable(x, LUA_REGISTRYINDEX);
#endif

#ifndef WINBUILD
#define Sleep(x) usleep(x)
void Beep(int freq, int dur);
#define MAX_PATH 1024
//WINBASEAPI WINBOOL WINAPI ReadProcessMemory (HANDLE hProcess, char* lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead);
//WINBASEAPI char* WINAPI VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
#else
#include <windows.h>
// global Hinstance for the DLL
extern HINSTANCE g_hInstance;
#endif // WINBUILD

#ifdef DBG_MESSAGES
#ifdef VERBOSE_DBG
#define EXTRA_DEBUG_MESSAGES
#endif
#endif

struct AMM_Magic_Header
{
	const char* ArmIllegalOp = 0x00000000;
	const int IntelIllegalOp = 0x0f0b;
	const char* version = AMM_VERSION; //zero-terminated C style ascii string.
	const char* HeaderMagicString = "AMM0";
	const long long int Magic = 0x1FF9E6F4D5C3B2B7; // An 8 byte value with relatively high entropy that is also prime
	void* memoryspacelistaddr; // pointer to the list of active memoryspace objects
	void* pagelistaddr; // pointer to the list of active page objects
	volatile char InterprocessChunk[256];
	volatile char ChunkReady;
};

#endif // DEFINES_H_INCLUDED
