
#include "AMMlib.h"
#include "LuaMemoryMapperLib.h"


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}

DLL_EXPORT unordered_map<string,virtualMemorySpace*>* libGetMemorySpaceList()
{
    return getMemorySpaceList();
}
DLL_EXPORT unordered_map<string,addressSpace*>* libGetMemoryPageList()
{
    return getMemoryPageList();
}

string DLL_EXPORT libCreateMemoryContext(string ID)
{
    return createMemoryContext(ID);
}
void DLL_EXPORT libDestroyMemoryContext(string ID)
{
    return destroyMemoryContext(ID);
}

void DLL_EXPORT libReadMemFromContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    return readMemFromContext(ContextID,Address,length,buf);
}
void DLL_EXPORT libWriteMemToContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    return writeMemToContext(ContextID,Address,length,buf);
}

void DLL_EXPORT libLinkPageToMemorySpace(string MemorySpaceName,string PageName)
{
    linkPageToMemorySpace(MemorySpaceName,PageName);
}
void DLL_EXPORT libDestroyPage(string ID)
{
    destroyPage(ID);
}

void DLL_EXPORT libAddVirtualPage(string ID,unsigned int PageSize)
{
    addVirtualPage(ID,PageSize);
}

void DLL_EXPORT libAddInterprocessPage(string ID,unsigned int PageSize,string windowName,int address)
{
    addInterprocessPage(ID,PageSize,windowName,address);
}
void DLL_EXPORT libAddMultiPage(string ID, unsigned int pagelistSize, string** pagelist)
{
    addMultiPage(ID,pagelistSize,pagelist);
}
void DLL_EXPORT libAddMetaPage(string ID, unsigned int PageSize,string SubMemorySpaceName,int address)
{
    addMetaPage(ID, PageSize,SubMemorySpaceName,address);
}
void DLL_EXPORT libAddLuaPage(string ID, string luafile)
{
    addLuaPage(ID,luafile);
}
void DLL_EXPORT libAddLoggedPage(string ID, string logfile, string pageID)
{
    addLoggedPage(ID, logfile, pageID);
}
void DLL_EXPORT libAddFilePage(string ID,string filename,unsigned int length)
{
    addFilePage(ID,filename,length);
}

void DLL_EXPORT libLua_RegisterMemoryFunctions(lua_State* L)
{
    lua_RegisterMemoryFunctions(L);
}
void DLL_EXPORT libLua_handle_error(lua_State* L,int errcode)
{
    lua_handle_error(L,errcode);
}
