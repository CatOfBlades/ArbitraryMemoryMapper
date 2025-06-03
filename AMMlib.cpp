
#include "AMMlib.h"
#include "LuaMemoryMapperLib.h"

#include "Defines.h"
HINSTANCE g_hInstance = nullptr;

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    HANDLE hProcess=GetCurrentProcess();
    HANDLE hToken;
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
            {
                SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
                CloseHandle(hToken);
            }

            g_hInstance = hinstDLL;
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
DLL_EXPORT std::unordered_map<std::string,std::shared_ptr<addressSpace>>* libGetMemoryPageList()
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
/*
void DLL_EXPORT libLua_RegisterMemoryFunctions(lua_State* L)
{
    lua_RegisterMemoryFunctions(L);
}
*/
void DLL_EXPORT libLua_handle_error(lua_State* L,int errcode)
{
    lua_handle_error(L,errcode);
}

/*
    static const struct luaL_Reg ammlib [] = {
        {"createMemoryContext",lua_CreateMemoryContext},
        {"destroyMemoryContext",lua_DestroyMemoryContext},
        {"SysBeep",lua_Beep},
        {"Sleep",lua_Sleep},

        {"addVirtualPage",lua_addVirtualPage},
        #ifdef WINBUILD
        {"addInterprocessPage",lua_addInterprocessPage},
        #endif // WINBUILD

        {"addMultiPage",lua_addMultiPage},
        {"addMetaPage",lua_addMetaPage},
        {"addLuaPage",lua_addLuaPage},
        {"addLoggedPage",lua_addLoggedPage},
        {"addFilePage",lua_addFilePage},
        #ifdef BUILD_WIN_MEMACCESSOR
        {"addMPA_Page",lua_addMPA_Page},
        #endif //BUILD_WIN_MEMACCESSOR

        {"linkPageToMemorySpace",lua_linkPageToMemorySpace},
        {"destroyPage",lua_destroyPage},
        {"readMemFromContext",lua_readMemFromContext},
        {"writeMemToContext",lua_writeMemToContext},
        {"multiPageSwapBanks",lua_multiPageSwapBanks},
        {NULL, NULL}
    };
*/

extern "C" __declspec(dllexport) int luaopen_ammlib(lua_State *L)
{
    lua_RegisterMemoryFunctions(L);
    //lua_register(L,"createMemoryContext",lua_CreateMemoryContext);
    //Sleep(10);
    //luaL_newlib(L, ammlib);
    //lua_pushnumber(L, 3141);
    //lua_setfield(L, -2, "ammpi");

    return 1;
}


