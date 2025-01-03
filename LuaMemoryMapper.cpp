
#include "LuaMemoryMapperLib.h"

/**
So what I want to do with this is make an application that takes a .lua file and produces an active memory setup.
This way I can craft memory mappings without having to recompile.
Also it could act as a language to algorithmically produce mappings.
With this we could define things in terms of mappings.
Arbitrarily mapping between one context and another.
These contexts being filetypes, videogame memories or any context that defines variables in a memory space.
**/

#ifdef WINBUILD
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
#endif // WINBUILD

 /******************************************************************************\
                                int main()
 \******************************************************************************/

#ifdef DBG_MESSAGES
#ifdef LINUX_BUILD
std::string BuildInfo = "hello world";
#else
#include "revinfo.h"
#endif // LINUX_BUILD
#endif

int main(int argc,char* argv[])
{

    AMM_Magic_Header _AMMMagicHeader;

    if( (argc != 1)&&(argc != 2) ){}; //!TODO: make the OpenGL window only appear when a flag is set. Maybe -R for render?

#ifdef DBG_MESSAGES
    printf("%s\n", BuildInfo.c_str());
#endif
#ifdef WINBUILD
/*
    HANDLE hProcess=GetCurrentProcess();
    HANDLE hToken;

    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
        {
            CloseHandle(hToken);
            //return 1;
        }
        CloseHandle(hToken);
    }
    */
#endif // WINBUILD

    //Beep(600,100);
    lua_State* L = luaL_newstate();
    if(!L)
    {
    #ifdef WINBUILD
        Beep(600,100);
    #else

    #endif // WINBUILD
        return -1;
    }
    luaL_openlibs(L);

    lua_RegisterMemoryFunctions(L);

    int r = luaL_dofile(L, "MemoryMap.lua");
    lua_handle_error(L, r);
    return 0;
}
