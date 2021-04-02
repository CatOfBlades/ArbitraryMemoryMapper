


/**
So what I want to do with this is make an application that takes a .lua file and produces an active memory setup.
This way I can craft memory mappings without having to recompile.
Also it could act as a language to algorithmically produce mappings.
With this we could define things in terms of mappings.
Arbitrarily mapping between one context and another.
These contexts being filetypes, videogame memories or any context that defines variables in a memory space.
**/

#include "LuaMemoryMapperLib.h"

unordered_map<string,virtualMemorySpace*> memorySpaces; //This is the memory contexts we have opened with createMemoryContext()
unordered_map<string,addressSpace*> memoryPages;

/**
for creating interfaces on the C++ side we need a way to expose the memory space and address space pointers
**/
unordered_map<string,virtualMemorySpace*>* getMemorySpaceList()
{
    return &memorySpaces;
}
unordered_map<string,addressSpace*>* getMemoryPageList()
{
    return &memoryPages;
}
/** **/

/******************************************************************************\
                                int main()

int main()
{

#ifdef WINBUILD
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
#endif // WINBUILD

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_RegisterMemoryFunctions(L);
    int r = luaL_dofile(L, "MemoryMap.lua");
    lua_handle_error(L, r);
    return 0;
}

\******************************************************************************/


void lua_RegisterMemoryFunctions(lua_State* L)
{
    lua_register(L,"createMemoryContext",lua_CreateMemoryContext);
    lua_register(L,"destroyMemoryContext",lua_DestroyMemoryContext);
    lua_register(L,"SysBeep",lua_Beep);
    lua_register(L,"Sleep",lua_Sleep);
    lua_register(L,"addVirtualPage",lua_addVirtualPage);
    #ifdef WINBUILD
    lua_register(L,"addInterprocessPage",lua_addInterprocessPage);
    #endif // WINBUILD
    lua_register(L,"addMultiPage",lua_addMultiPage);
    lua_register(L,"addMetaPage",lua_addMetaPage);
    lua_register(L,"addLuaPage",lua_addLuaPage);
    lua_register(L,"addLoggedPage",lua_addLoggedPage);
    lua_register(L,"linkPageToMemorySpace",lua_linkPageToMemorySpace);
    lua_register(L,"destroyPage",lua_destroyPage);
    lua_register(L,"readMemFromContext",lua_readMemFromContext);
    lua_register(L,"writeMemToContext",lua_writeMemToContext);
    lua_register(L,"multiPageSwapBanks",lua_multiPageSwapBanks);
}

string createMemoryContext(string ID)
{
    pair<unordered_map<string,virtualMemorySpace*>::iterator,bool> IB;
    virtualMemorySpace* VM = new virtualMemorySpace();
    pair <string,virtualMemorySpace*> valu(ID,VM);
    IB = memorySpaces.insert(valu);
    if(IB.second)
    {
        return ID;
    }
    else
    {
        delete VM;
        return ID;
    }
}
void destroyMemoryContext(string ID)
{
    if(memorySpaces.find(ID)!=memorySpaces.end())
    {
        delete memorySpaces.at(ID);
        memorySpaces.erase(ID);
    }
    else
    {
        return;
    }
}

void linkPageToMemorySpace(string MemorySpaceName,string PageName)
{
    addressSpace* AS = memoryPages.at(PageName);
    //Sleep(100);
    //printf("memorySpaceName:%s\n",MemorySpaceName.c_str());
    memorySpaces.at(MemorySpaceName)->addAddressSpace(AS);
}

void destroyPage(string ID)
{
    if(memoryPages.find(ID)!=memoryPages.end())
    {
        memoryPages.at(ID)->cleanupAndUnlink();
        memoryPages.erase(ID);
    }
    else
    {
        return;
    }
}

void addVirtualPage(string ID,int PageSize)
{
    addressSpace* AS = new virtualPage(NULL,(unsigned int)PageSize);

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair<string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
    return;
}

#ifdef WINBUILD
void addInterprocessPage(string ID,int PageSize,string windowName,int address)
{
    DWORD processid = NULL;
    HWND hw = FindWindow(NULL, windowName.c_str());
    GetWindowThreadProcessId(hw, &processid);
    if(processid == NULL)
    {
        //Beep(350,100);
    }
    else
    {
        //Beep(600,100);
    }
    addressSpace* AS = new applicationMem(NULL, processid, (unsigned int)PageSize,(HANDLE)address);

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair <string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
    return;
}
#endif // WINBUILD

void addMultiPage(string ID, int pagelistSize,string** pagelist)
{

    multiPage* AS = new multiPage();

    while(pagelistSize>0)
    {
        AS->addBank(memoryPages.at(*pagelist[pagelistSize-1]));
        pagelistSize-=1;
    }

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair <string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
    return;
}

void addMetaPage(string ID,int PageSize,string SubMemorySpaceName,int address)
{
    addressSpace* AS = new memorySpacePage(memorySpaces.at(SubMemorySpaceName),address, (unsigned int)PageSize);

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair <string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
}
void addLuaPage(string ID, string luafile) //Lua defined memory page.
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_RegisterMemoryFunctions(L);
    luaPage* AS = new luaPage(L,NULL,luafile);
    AS->freeLuaOnDestruct = 1;

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair <string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
}
void addLoggedPage(string ID, string logfile, string pageID)
{
    loggedPage* AS = new loggedPage(logfile,memoryPages.at(pageID));

    pair<unordered_map<string,addressSpace*>::iterator,bool> IB;
    pair <string,addressSpace*> valu(ID,AS);
    IB = memoryPages.insert(valu);
    if(IB.second)
    {
        return;
    }
    else
    {
        delete AS; //if theres already a page with the name we don't need the new one.
        return;
    }
}

void readMemFromContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    memorySpaces.at(ContextID)->readMem(Address,buf,length);
}
void writeMemToContext(string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    memorySpaces.at(ContextID)->writeMem(Address,buf,length);
}

int lua_CreateMemoryContext(lua_State* L)
{
    string ID = lua_tostring(L,1);
    ID = createMemoryContext(ID);
    lua_pushstring(L,ID.c_str());
    return 1;
}
int lua_DestroyMemoryContext(lua_State* L)
{
    string ID = lua_tostring(L,1);
    destroyMemoryContext(ID);
    return 0;
}
int lua_Beep(lua_State* L)
{
    int a = lua_tointeger(L,1);
    int b = lua_tointeger(L,2);
    #ifdef WINBUILD
    Beep(a,b);
    #else
    system("echo $'\a'");
    #endif //WINBUILD
    return 0;
}
int lua_Sleep(lua_State* L)
{
    int a = lua_tointeger(L,1);
    #ifdef WINBUILD
    Sleep(a);
    #else
    usleep(a*1000);
    #endif //WINBUILD
    return 0;
}

int lua_addVirtualPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    int pageSize = lua_tointeger(L,2);
    //printf("PageID:%s\npageSize:%i\n",ID.c_str(),pageSize);
    addVirtualPage(ID,pageSize);
    return 0;
}
#ifdef WINBUILD
int lua_addInterprocessPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    int pageSize = lua_tointeger(L,2);
    string WindowName = lua_tostring(L,3);
    int address = lua_tointeger(L,4);
    addInterprocessPage(ID,pageSize,WindowName,(unsigned int)address);
    return 0;
}
#endif // WINBUILD
int lua_addMultiPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    //int pageSize = lua_tointeger(L,2);
    int pagelistSize = lua_tointeger(L,2);
    string* namelist[pagelistSize];
    if(lua_istable(L,3))
    {
        /*
        for (int i=1; i<=pagelistSize; i++)
        {
            lua_pushinteger(L,i);
            lua_gettable(L,3);
            namelist[i-1] = new string(lua_tostring(L,-1));
            lua_pop(L,-1);
        }
        */

        lua_pushnil(L);  /* first key */
        int PLS=0;
        while (lua_next(L, 3) != 0)
        {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            int a = lua_tonumber(L,-2);
            namelist[a-1] = new string(lua_tostring(L,-1));
            lua_pop(L, 1);
            PLS++;
            //printf(namelist[a-1]->c_str()); ///   dbg   jjj
        }

        if(PLS!=pagelistSize)
        {
            printf("System Warning: size of list(value:%i) was not equal to the pageListSize(value:%i) variable.\nSome data could have been corrupted.",PLS,pagelistSize);
        }

        addMultiPage(ID,PLS,namelist);
        for (int i=1; i<=pagelistSize; i++)
        {
            delete namelist[i-1];
        }
    }
    else
    {
        return 0;
    }
    //string* pagelist = lua_to;
    return 0;
}
int lua_addMetaPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    int pageSize = lua_tointeger(L,2);
    string SubMemorySpaceName = lua_tostring(L,3);
    int address = lua_tointeger(L,4);
    addMetaPage(ID,pageSize,SubMemorySpaceName,address);
    return 0;
}
int lua_addLuaPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    string fileName = lua_tostring(L,2);
    addLuaPage(ID,fileName);
    return 0;
}
int lua_addLoggedPage(lua_State* L)
{
    //(string ID, string logfile, string pageID)
    string ID = lua_tostring(L,1);
    string fileName = lua_tostring(L,2);
    string pageID = lua_tostring(L,3);
    addLoggedPage(ID,fileName,pageID);
    return 0;
}
int lua_linkPageToMemorySpace(lua_State* L)
{
    string memorySpace_ID = lua_tostring(L,1);
    string page_ID = lua_tostring(L,2);
    //printf("MemorySpace:%s\n page_ID:%s\n ",memorySpace_ID.c_str(),page_ID.c_str());
    linkPageToMemorySpace(memorySpace_ID,page_ID);
    return 0;
}
int lua_destroyPage(lua_State* L)
{
    string ID = lua_tostring(L,1);
    destroyPage(ID);
    return 0;
}

int lua_readMemFromContext(lua_State* L)
{
string ContextID = lua_tostring(L,1);
unsigned long int Address = lua_tointeger(L,2);
unsigned long int length = lua_tointeger(L,3);
unsigned char buf[length];

    //printf("ContextID:%s\n address:%i\n length:%i\n",ContextID.c_str(),Address,length);
    readMemFromContext(ContextID,Address,length,buf);
    //printf("val1:%i,val2:%i,val3:%i\n",buf[0],buf[1],buf[2]);
    //Beep(400,100);
    lua_createtable(L,0,length);
    for (int i=1; i<=length; i++)
    {
        //lua_pushinteger(L,i);
        lua_pushinteger(L, buf[i-1]);
        lua_rawseti(L, -2,i);
        //lua_settable(L, -3);
    }
    return 1;
}

int lua_writeMemToContext(lua_State* L)
{
string ContextID = lua_tostring(L,1);
unsigned long int Address = lua_tointeger(L,2);
unsigned long int length = lua_tointeger(L,3);
unsigned char buf[length];

    if(lua_istable(L,4))
    {
        /* table is in the stack at index 't' */
        lua_pushnil(L);  /* first key */
        while (lua_next(L, 4) != 0)
        {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            int a = lua_tonumber(L,-2);
            int b = lua_tonumber(L,-1);
            //printf("%i - %i\n",a,b);
            buf[a-1] = b;
            lua_pop(L, 1);
        }

        writeMemToContext(ContextID,Address,length,buf);
        return 0;
    }
    else
    {
        return 0;
    }
}

int lua_multiPageSwapBanks(lua_State* L)
{
    string ID = lua_tostring(L,1);
    int bankNum = lua_tonumber(L,2);
    if( memoryPages.at(ID)->memoryTypeID == "multpage" )
    {
        //printf("banknum:%i\n",bankNum);
        ((multiPage*)memoryPages.at(ID))->switchBank(bankNum);
    }
    return 0;
}

void lua_handle_error(lua_State* L,int errcode)
{
    if(errcode==LUA_OK)
    {
        return;
    }
    else
    {
        string out = lua_tostring(L,-1);
        printf("lua error: %s\n",out.c_str());
    }
}

#ifdef WINBUILD
//
//  SetPrivilege enables/disables process token privilege.
//
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    LUID luid;
    BOOL bRet=FALSE;

    if (LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
    {
        TOKEN_PRIVILEGES tp;

        tp.PrivilegeCount=1;
        tp.Privileges[0].Luid=luid;
        tp.Privileges[0].Attributes=(bEnablePrivilege) ? SE_PRIVILEGE_ENABLED: 0;
        //
        //  Enable the privilege or disable all privileges.
        //
        if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
        {
            //
            //  Check to see if you have proper access.
            //  You may get "ERROR_NOT_ALL_ASSIGNED".
            //
            bRet=(GetLastError() == ERROR_SUCCESS);
        }
    }
    return bRet;
}
#endif //WINBUILD
