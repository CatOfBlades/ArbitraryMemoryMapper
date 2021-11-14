#ifndef AMMLIB_H_INCLUDED
#define AMMLIB_H_INCLUDED


#include <windows.h>
#include <unordered_map>
#include <string>
#include "AMM/virtualMemorySpace.h"
#include "AMM/addressSpace.h"
#include "lua.h"

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


DLL_EXPORT std::unordered_map<std::string,virtualMemorySpace*>* libGetMemorySpaceList();
DLL_EXPORT std::unordered_map<std::string,addressSpace*>* libGetMemoryPageList();

std::string DLL_EXPORT libCreateMemoryContext(std::string ID);
void DLL_EXPORT libDestroyMemoryContext(std::string ID);

void DLL_EXPORT libReadMemFromContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);
void DLL_EXPORT libWriteMemToContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);

void DLL_EXPORT libLinkPageToMemorySpace(std::string MemorySpaceName,std::string PageName);
void DLL_EXPORT libDestroyPage(std::string ID);

void DLL_EXPORT libAddVirtualPage(std::string ID,unsigned int PageSize);
void DLL_EXPORT libAddInterprocessPage(std::string ID,unsigned int PageSize,std::string windowName,int address);
void DLL_EXPORT libAddMultiPage(std::string ID, unsigned int pagelistSize, std::vector<std::string*> pagelist);
void DLL_EXPORT libAddMetaPage(std::string ID, unsigned int PageSize,std::string SubMemorySpaceName,int address); //SelfRefrence.cpp
void DLL_EXPORT libAddLuaPage(std::string ID, std::string luafile); //Lua defined memory page.
void DLL_EXPORT libAddLoggedPage(std::string ID, std::string logfile, std::string pageID);
void DLL_EXPORT libAddFilePage(std::string ID,std::string filename,unsigned int length);

void DLL_EXPORT libLua_RegisterMemoryFunctions(lua_State* L);
void DLL_EXPORT libLua_handle_error(lua_State* L,int errcode);

#ifdef __cplusplus
extern "C"
{
#endif

//void DLL_EXPORT SomeFunction(const LPCSTR sometext);

#ifdef __cplusplus
}
#endif


#endif // AMMLIB_H_INCLUDED
