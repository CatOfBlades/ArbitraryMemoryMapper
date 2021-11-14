
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

DLL_EXPORT std::unordered_map<std::string,virtualMemorySpace*>* libGetMemorySpaceList()
{
    return getMemorySpaceList();
}
DLL_EXPORT std::unordered_map<std::string,addressSpace*>* libGetMemoryPageList()
{
    return getMemoryPageList();
}

std::string DLL_EXPORT libCreateMemoryContext(std::string ID)
{
    return createMemoryContext(ID);
}
void DLL_EXPORT libDestroyMemoryContext(std::string ID)
{
    return destroyMemoryContext(ID);
}

void DLL_EXPORT libReadMemFromContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    return readMemFromContext(ContextID,Address,length,buf);
}
void DLL_EXPORT libWriteMemToContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    return writeMemToContext(ContextID,Address,length,buf);
}

void DLL_EXPORT libLinkPageToMemorySpace(std::string MemorySpaceName,std::string PageName)
{
    linkPageToMemorySpace(MemorySpaceName,PageName);
}
void DLL_EXPORT libDestroyPage(std::string ID)
{
    destroyPage(ID);
}

void DLL_EXPORT libAddVirtualPage(std::string ID,unsigned int PageSize)
{
    addVirtualPage(ID,PageSize);
}

void DLL_EXPORT libAddInterprocessPage(std::string ID,unsigned int PageSize,std::string windowName,char* address)
{
    addInterprocessPage(ID,PageSize,windowName,address);
}
void DLL_EXPORT libAddMultiPage(std::string ID, unsigned int pagelistSize, std::vector<std::string*> pagelist)
{
    addMultiPage(ID,pagelistSize,pagelist);
}
void DLL_EXPORT libAddMetaPage(std::string ID, unsigned int PageSize,std::string SubMemorySpaceName,char* address)
{
    addMetaPage(ID, PageSize,SubMemorySpaceName,address);
}
void DLL_EXPORT libAddLuaPage(std::string ID, std::string luafile)
{
    addLuaPage(ID,luafile);
}
void DLL_EXPORT libAddLoggedPage(std::string ID, std::string logfile, std::string pageID)
{
    addLoggedPage(ID, logfile, pageID);
}
void DLL_EXPORT libAddFilePage(std::string ID,std::string filename,unsigned int length)
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
