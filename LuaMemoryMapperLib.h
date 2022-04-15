
#include <iostream>
#include <stdio.h>
//#include <libdis.h>
#include "Defines.h"

#ifdef WINBUILD
#include <Windows.h>
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

#ifdef BUILD_WIN_MEMACCESSOR
#include "AMM/WindowsPageAccessor.h"
#endif // BUILD_WIN_MEMACCESSOR

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

/**
for creating interfaces on the C++ side we need a way to expose the memory space and address space pointers
**/
std::unordered_map<std::string,virtualMemorySpace*>* getMemorySpaceList();
std::unordered_map<std::string,std::shared_ptr<addressSpace>>* getMemoryPageList();
/** **/

std::string createMemoryContext(std::string ID);
char* cstr_createMemoryContext(char*);
void destroyMemoryContext(std::string ID);

void readMemFromContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);
void writeMemToContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);

void linkPageToMemorySpace(std::string MemorySpaceName,std::string PageName);
void destroyPage(std::string ID);

void addVirtualPage(std::string ID,unsigned int PageSize);
void addInterprocessPage(std::string ID,unsigned int PageSize,std::string windowName,char* address);
void addMultiPage(std::string ID, unsigned int pagelistSize, std::vector<std::string*> pagelist);
void addMetaPage(std::string ID, unsigned int PageSize,std::string SubMemorySpaceName,char* address); //SelfRefrence.cpp
void addLuaPage(std::string ID, std::string luafile); //Lua defined memory page.
void addLoggedPage(std::string ID, std::string logfile, std::string pageID);
void addFilePage(std::string ID,std::string filename,unsigned int length);

#ifdef SUPPORT_UNUSUAL_MEMORY_ACCESSES
float splitByteRW(std::string ContextID, bool write,unsigned long address1,unsigned long address2, char byt, float rw_ratio);
#endif // SUPPORT_UNUSUAL_MEMORY_ACCESSES

#ifdef BUILD_WIN_MEMACCESSOR
void addMPA_Page(std::string ID);
#endif //BUILD_WIN_MEMACCESSOR

extern "C"
{
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
    #ifdef BUILD_WIN_MEMACCESSOR
    int lua_addMPA_Page(lua_State* L);
    #endif //BUILD_WIN_MEMACCESSOR

    int lua_linkPageToMemorySpace(lua_State* L);
    int lua_destroyPage(lua_State* L);
    int lua_readMemFromContext(lua_State* L);
    int lua_writeMemToContext(lua_State* L);
    int lua_multiPageSwapBanks(lua_State* L);

    void lua_RegisterMemoryFunctions(lua_State* L);


    void lua_handle_error(lua_State* L,int errcode);

    #ifdef SUPPORT_UNUSUAL_MEMORY_ACCESSES
    int lua_splitByteRW(lua_State* L);
    #endif // SUPPORT_UNUSUAL_MEMORY_ACCESSES

}

#ifdef WINBUILD
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
#endif // WINBUILD
