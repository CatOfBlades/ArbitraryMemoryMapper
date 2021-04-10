
#include <iostream>
#include <stdio.h>
//#include <libdis.h>
#include "Defines.h"

#ifdef WINBUILD
#include <windows.h>
#else
#include <unistd.h>
#endif // WINBUILD

#include <string>
#include <vector>
#include <unordered_map>

#ifdef WINBUILD
#define USINGINTERPROCESS
#endif // WINBUILD
#define USING_SELF_REFRENCE
#define USING_MULTI_PAGE
#define USING_LUA_PAGE
#define USINGPAGELOGGING
#define USING_FILE_PAGE
#include "ArbitraryMemMap.h"
#undef USINGINTERPROCESS
#undef USING_SELF_REFRENCE
#undef USING_MULTI_PAGE
#undef USING_LUA_PAGE
#undef USINGPAGELOGGING
#undef USING_FILE_PAGE

using namespace std;

extern "C"
{
    #include "lua-5.4.0/src/lua.h"
    #include "lua-5.4.0/src/lauxlib.h"
    #include "lua-5.4.0/src/lualib.h"
}

/**
for creating interfaces on the C++ side we need a way to expose the memory space and address space pointers
**/
unordered_map<string,virtualMemorySpace*>* getMemorySpaceList();
unordered_map<string,addressSpace*>* getMemoryPageList();
/** **/

string createMemoryContext(string ID);
void destroyMemoryContext(string ID);

void readMemFromContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);
void writeMemToContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);

void linkPageToMemorySpace(string MemorySpaceName,string PageName);
void destroyPage(string ID);

void addVirtualPage(string ID,unsigned int PageSize);
void addInterprocessPage(string ID,unsigned int PageSize,string windowName,int address);
void addMultiPage(string ID, unsigned int pagelistSize, string** pagelist);
void addMetaPage(string ID, unsigned int PageSize,string SubMemorySpaceName,int address); //SelfRefrence.cpp
void addLuaPage(string ID, string luafile); //Lua defined memory page.
void addLoggedPage(string ID, string logfile, string pageID);
void addFilePage(string ID,string filename,unsigned int length);

int lua_CreateMemoryContext(lua_State* L);
int lua_DestroyMemoryContext(lua_State* L);
int lua_Beep(lua_State* L);
int lua_Sleep(lua_State* L);

int lua_addVirtualPage(lua_State* L);
int lua_addInterprocessPage(lua_State* L);
int lua_addMultiPage(lua_State* L);
int lua_addMetaPage(lua_State* L);
int lua_addLuaPage(lua_State* L);
int lua_addLoggedPage(lua_State* L);
int lua_addFilePage(lua_State* L);

int lua_linkPageToMemorySpace(lua_State* L);
int lua_destroyPage(lua_State* L);
int lua_readMemFromContext(lua_State* L);
int lua_writeMemToContext(lua_State* L);
int lua_multiPageSwapBanks(lua_State* L);

void lua_RegisterMemoryFunctions(lua_State* L);

void lua_handle_error(lua_State* L,int errcode);

#ifdef WINBUILD
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
#endif // WINBUILD
