


/**
So what I want to do with this is make an application that takes a .lua file and produces an active memory setup.
This way I can craft memory mappings without having to recompile.
Also it could act as a language to algorithmically produce mappings.
With this we could define things in terms of mappings.
Arbitrarily mapping between one context and another.
These contexts being filetypes, videogame memories or any context that defines variables in a memory space.
**/

#include "LuaMemoryMapperLib.h"
#include <vector>

std::unordered_map<std::string,virtualMemorySpace*> memorySpaces; //This is the memory contexts we have opened with createMemoryContext()
std::unordered_map<std::string,addressSpace*> memoryPages;

/**
for creating interfaces on the C++ side we need a way to expose the memory space and address space pointers
**/
std::unordered_map<std::string,virtualMemorySpace*>* getMemorySpaceList()
{
    return &memorySpaces;
}
std::unordered_map<std::string,addressSpace*>* getMemoryPageList()
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

extern "C"
{

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
        lua_register(L,"addFilePage",lua_addFilePage);
        #ifdef BUILD_WIN_MEMACCESSOR
        lua_register(L,"addMPA_Page",lua_addMPA_Page);
        #endif //BUILD_WIN_MEMACCESSOR

        lua_register(L,"linkPageToMemorySpace",lua_linkPageToMemorySpace);
        lua_register(L,"destroyPage",lua_destroyPage);
        lua_register(L,"readMemFromContext",lua_readMemFromContext);
        lua_register(L,"writeMemToContext",lua_writeMemToContext);
        lua_register(L,"multiPageSwapBanks",lua_multiPageSwapBanks);

        #ifdef SUPPORT_UNUSUAL_MEMORY_ACCESSES
        lua_register(L,"splitByteRW",lua_splitByteRW);
        //float splitByteRW(bool write,unsigned long address, char byt);
        //lua_splitByteRW
        #endif // SUPPORT_UNUSUAL_MEMORY_ACCESSES

    }

}
std::string createMemoryContext(std::string ID)
{
    std::pair<std::unordered_map<std::string,virtualMemorySpace*>::iterator,bool> IB;
    virtualMemorySpace* VM = new virtualMemorySpace();
    std::pair <std::string,virtualMemorySpace*> valu(ID,VM);
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

const char* cstr_createMemoryContext(const char* ID)
{
    std::pair<std::unordered_map<std::string,virtualMemorySpace*>::iterator,bool> IB;
    virtualMemorySpace* VM = new virtualMemorySpace();
    std::pair <std::string,virtualMemorySpace*> valu(ID,VM);
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
void destroyMemoryContext(std::string ID)
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

void linkPageToMemorySpace(std::string MemorySpaceName,std::string PageName)
{
    addressSpace* AS = memoryPages.at(PageName);
    //Sleep(100);
    //printf("memorySpaceName:%s\n",MemorySpaceName.c_str());
    virtualMemorySpace* vm;
    try
    {
        vm = memorySpaces.at(MemorySpaceName);
    }
    catch (const std::out_of_range& oor)
    {
        printf("memorySpaceName:%s not found in list\n",MemorySpaceName.c_str());
        return;
    }
    vm->addAddressSpace(AS);
}

void destroyPage(std::string ID)
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

void addVirtualPage(std::string ID,unsigned int PageSize)
{
    addressSpace* AS = new virtualPage(NULL,(unsigned int)PageSize);

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair<std::string,addressSpace*> valu(ID,AS);
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
void addInterprocessPage(std::string ID,unsigned int PageSize,std::string windowName,char* address)
{
    DWORD processid = 0;
    HWND hw = FindWindow(0, windowName.c_str());
    GetWindowThreadProcessId(hw, &processid);
    if(processid == 0)
    {
        //Beep(350,100);
    }
    else
    {
        //Beep(600,100);
    }
    addressSpace* AS = new applicationMem(0, processid, PageSize,address);

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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

void addMultiPage(std::string ID, unsigned int pagelistSize,std::vector<std::string*> pagelist)
{

    multiPage* AS = new multiPage();

    while(pagelistSize>0)
    {
        AS->addBank(memoryPages.at(*pagelist[pagelistSize-1]));
        pagelistSize-=1;
    }

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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

void addMetaPage(std::string ID,unsigned int PageSize,std::string SubMemorySpaceName,char* address)
{
    addressSpace* AS = new memorySpacePage(memorySpaces.at(SubMemorySpaceName),address, (unsigned int)PageSize);

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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
void addLuaPage(std::string ID, std::string luafile) //Lua defined memory page.
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_RegisterMemoryFunctions(L);
    luaPage* AS = new luaPage(L,NULL,luafile);
    AS->freeLuaOnDestruct = 1;

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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
void addLoggedPage(std::string ID, std::string logfile, std::string pageID)
{
    loggedPage* AS = new loggedPage(logfile,memoryPages.at(pageID));

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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

void addFilePage(std::string ID,std::string filename,unsigned int length)
{
    filePage* AS = new filePage(filename,length);

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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

#ifdef BUILD_WIN_MEMACCESSOR
void addMPA_Page(std::string ID)
{
    MPA_page* AS = new MPA_page();

    std::pair<std::unordered_map<std::string,addressSpace*>::iterator,bool> IB;
    std::pair <std::string,addressSpace*> valu(ID,AS);
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
#endif //BUILD_WIN_MEMACCESSOR


void readMemFromContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    memorySpaces.at(ContextID)->readMem(Address,buf,length);
}
void writeMemToContext(std::string ContextID,unsigned long int Address,unsigned long int length,unsigned char* buf)
{
    memorySpaces.at(ContextID)->writeMem(Address,buf,length);
}

extern "C"
{

    int lua_CreateMemoryContext(lua_State* L)
    {
        const char * ID = lua_tostring(L,1);
        //ID = createMemoryContext(ID);
        //lua_pushstring(L,ID.c_str());
        lua_pushstring(L,cstr_createMemoryContext(ID));
        return 1;
    }
    int lua_DestroyMemoryContext(lua_State* L)
    {
        const char *  ID = lua_tostring(L,1);
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
        const char *  ID = lua_tostring(L,1);
        int pageSize = lua_tointeger(L,2);
        //printf("PageID:%s\npageSize:%i\n",ID.c_str(),pageSize);
        addVirtualPage(ID,pageSize);
        return 0;
    }
    #ifdef WINBUILD
    int lua_addInterprocessPage(lua_State* L)
    {
        const char *  ID = lua_tostring(L,1);
        int pageSize = lua_tointeger(L,2);
        const char *  WindowName = lua_tostring(L,3);
        char* address = 0;
        address=address+lua_tointeger(L,4);
        addInterprocessPage(ID,pageSize,WindowName,address);
        return 0;
    }
    #endif // WINBUILD
}
int lua_addMultiPage(lua_State* L)
{
    std::string ID = lua_tostring(L,1);
    //int pageSize = lua_tointeger(L,2);
    int pagelistSize = lua_tointeger(L,2);
    std::vector<std::string*> namelist;
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
            const std::vector<std::string*>::iterator a = namelist.begin()+(lua_tonumber(L,-2)-1);
            namelist.emplace(a,new std::string(lua_tostring(L,-1)));
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
    std::string ID = lua_tostring(L,1);
    int pageSize = lua_tointeger(L,2);
    std::string SubMemorySpaceName = lua_tostring(L,3);
    char* address = 0;
    address=address+lua_tointeger(L,4);
    addMetaPage(ID,pageSize,SubMemorySpaceName,address);
    return 0;
}
int lua_addLuaPage(lua_State* L)
{
    std::string ID = lua_tostring(L,1);
    std::string fileName = lua_tostring(L,2);
    addLuaPage(ID,fileName);
    return 0;
}
int lua_addLoggedPage(lua_State* L)
{
    //(string ID, string logfile, string pageID)
    std::string ID = lua_tostring(L,1);
    std::string fileName = lua_tostring(L,2);
    std::string pageID = lua_tostring(L,3);
    addLoggedPage(ID,fileName,pageID);
    return 0;
}
int lua_addFilePage(lua_State* L)
{
    std::string ID  = lua_tostring(L,1);
    std::string filename = lua_tostring(L,2);
    unsigned int length = lua_tointeger(L,3);
    addFilePage(ID,filename,length);
    return 0;
}
int lua_addMPA_Page(lua_State* L)
{
    std::string ID  = lua_tostring(L,1);
    addMPA_Page(ID);
    return 0;
}
int lua_linkPageToMemorySpace(lua_State* L)
{
    std::string memorySpace_ID = lua_tostring(L,1);
    std::string page_ID = lua_tostring(L,2);
    //printf("MemorySpace:%s\n page_ID:%s\n ",memorySpace_ID.c_str(),page_ID.c_str());
    linkPageToMemorySpace(memorySpace_ID,page_ID);
    return 0;
}
int lua_destroyPage(lua_State* L)
{
    std::string ID = lua_tostring(L,1);
    destroyPage(ID);
    return 0;
}

int lua_readMemFromContext(lua_State* L)
{
    std::string ContextID = lua_tostring(L,1);
    unsigned long int Address = lua_tointeger(L,2);
    unsigned long int length = lua_tointeger(L,3);
    //unsigned char buf[length];
    unsigned char buf[MAX_PATH];

    //printf("ContextID:%s\n address:%i\n length:%i\n",ContextID.c_str(),Address,length);
    readMemFromContext(ContextID,Address,length,buf);
    //printf("val1:%i,val2:%i,val3:%i\n",buf[0],buf[1],buf[2]);
    //Beep(400,100);
    lua_createtable(L,0,length);
    for (unsigned long int i=1; i<=length; i++)
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
    std::string ContextID = lua_tostring(L,1);
    unsigned long int Address = lua_tointeger(L,2);
    unsigned long int length = lua_tointeger(L,3);
    //unsigned char buf[length];
    unsigned char buf[MAX_PATH];

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
    std::string ID = lua_tostring(L,1);
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
        std::string out = lua_tostring(L,-1);
        printf("lua error: %s\n",out.c_str());
    }
}

#ifdef SUPPORT_UNUSUAL_MEMORY_ACCESSES
float splitByteRW(std::string ContextID, bool write,unsigned long address1,unsigned long address2, char byt, float rw_ratio)
{
    //look I know subverting a cast with a union is a code smell.
    // I need to access the unsigned byte for reading and writing,
    // but having the unusual R/W function respect negative numbers is useful.
    union m{unsigned char a;char b;};
    m buf;
    buf.b =byt;
    m bufpart1, bufpart2;
    float inverseRatio = (1-rw_ratio);
    if(write)
    {
        memorySpaces.at(ContextID)->readMem(address1,&bufpart1.a,1);
        memorySpaces.at(ContextID)->readMem(address2,&bufpart2.a,1);
        float intermediate1 = ((float)bufpart1.b*rw_ratio)+((float)byt*inverseRatio);
        float intermediate2 = ((float)bufpart2.b*inverseRatio)+((float)byt*rw_ratio);
        buf.b = intermediate1;
        memorySpaces.at(ContextID)->writeMem(address1,&buf.a,1);
        buf.b = intermediate2;
        memorySpaces.at(ContextID)->writeMem(address2,&buf.a,1);
    }
    else
    {
        memorySpaces.at(ContextID)->readMem(address1,&bufpart1.a,1);
        memorySpaces.at(ContextID)->readMem(address2,&bufpart2.a,1);
        return ((float)bufpart1.b*rw_ratio)+((float)bufpart2.b*inverseRatio);
    }
    return 0;

}
int lua_splitByteRW(lua_State* L)
{
    std::string ContextID = lua_tostring(L,1);
    bool write = lua_toboolean(L,2);
    unsigned long address1 = lua_tointeger(L,3);
    unsigned long address2 = lua_tointeger(L,4);
    char byt = lua_tointeger(L,5);
    float rw_ratio = lua_tonumber(L,6);
    lua_pushnumber(L, splitByteRW(ContextID, write,address1,address2, byt, rw_ratio));
    return 1;
}
#endif // SUPPORT_UNUSUAL_MEMORY_ACCESSES

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
        if (AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
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
