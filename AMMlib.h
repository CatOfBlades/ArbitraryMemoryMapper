#ifndef AMMLIB_H_INCLUDED
#define AMMLIB_H_INCLUDED


#include <windows.h>
#include <unordered_map>
#include <string>
#include "AMM/virtualMemorySpace.h"
#include "AMM/addressSpace.h"
#include "lua.h"

using namespace std;

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


DLL_EXPORT unordered_map<string,virtualMemorySpace*>* libGetMemorySpaceList();
DLL_EXPORT unordered_map<string,addressSpace*>* libGetMemoryPageList();

string DLL_EXPORT libCreateMemoryContext(string ID);
void DLL_EXPORT libDestroyMemoryContext(string ID);

void DLL_EXPORT libReadMemFromContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);
void DLL_EXPORT libWriteMemToContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf);

void DLL_EXPORT libLinkPageToMemorySpace(string MemorySpaceName,string PageName);
void DLL_EXPORT libDestroyPage(string ID);

void DLL_EXPORT libAddVirtualPage(string ID,unsigned int PageSize);
void DLL_EXPORT libAddInterprocessPage(string ID,unsigned int PageSize,string windowName,int address);
void DLL_EXPORT libAddMultiPage(string ID, unsigned int pagelistSize, string** pagelist);
void DLL_EXPORT libAddMetaPage(string ID, unsigned int PageSize,string SubMemorySpaceName,int address); //SelfRefrence.cpp
void DLL_EXPORT libAddLuaPage(string ID, string luafile); //Lua defined memory page.
void DLL_EXPORT libAddLoggedPage(string ID, string logfile, string pageID);
void DLL_EXPORT libAddFilePage(string ID,string filename,unsigned int length);

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
