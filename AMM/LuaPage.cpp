
#include "LuaPage.h"
#include <string>
#include <stdio.h>


void lua_handle_page_error(lua_State* L,int errcode)
{
    if(errcode==LUA_OK)
    {
        return;
    }
    else
    {
        std::string out = lua_tostring(L,-1);
        printf("lua page error: %s\n",out.c_str());
    }
}


unsigned long int luaPage::_size()
{
    lua_getglobal(pageLuaState,"_size");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pcall(pageLuaState,0,1,0);
        return (unsigned long int)lua_tonumber(pageLuaState,-1);
    }
    return 0; // _size  isn't a function failback to zero.
}

bool luaPage::_is_free()
{
    lua_getglobal(pageLuaState,"_is_free");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pcall(pageLuaState,0,1,0);
        return lua_toboolean(pageLuaState,-1);
    }
    return 0; // _is_free  isn't a function failback to zero.
}

unsigned char* luaPage::_content()
{
    return 0; //I am not doing raw pointers to lua memory
}

unsigned char luaPage::readByte(unsigned long int offset)
{
    lua_getglobal(pageLuaState,"readByte");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pushnumber(pageLuaState,offset);
        lua_pcall(pageLuaState,1,1,0);
        return (unsigned char)lua_tonumber(pageLuaState,-1);
    }
    return 0; //readByte isn't a function failback to zero.
}
void luaPage::writeByte(unsigned long int offset,unsigned char Byt)
{
    lua_getglobal(pageLuaState,"writeByte");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pushnumber(pageLuaState,offset);
        lua_pushnumber(pageLuaState,Byt);
        lua_pcall(pageLuaState,2,0,0);
    }
    return; //writeByte isn't a function.
}

void luaPage::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
    lua_getglobal(pageLuaState,"readMem");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pushnumber(pageLuaState,offset);
        lua_pushnumber(pageLuaState,len);
        lua_pcall(pageLuaState,2,1,0);
        if(lua_istable(pageLuaState,-1))
        {
            int i = 0;
            lua_pushnil(pageLuaState);
            while(1)
            {
                if(lua_next(pageLuaState, -2)==0){break;}
                else
                {
                    if(lua_isnumber(pageLuaState,-1))
                    {
                        buffer[i] = (unsigned char)lua_tointeger(pageLuaState,-1);
                        lua_pop(pageLuaState,1);
                    }
                    i++;
                }
            }
        }
    }
}
void luaPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    lua_getglobal(pageLuaState,"writeMem");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pushnumber(pageLuaState,offset);
        //lua_pushnumber(pageLuaState,*Byt);

        lua_createtable(pageLuaState,0,len);
        for (int i=1; i<=len; i++)
        {
            lua_pushinteger(pageLuaState, Byt[i-1]);
            lua_rawseti(pageLuaState, -2,i);
        }

        lua_pushnumber(pageLuaState,len);
        lua_pcall(pageLuaState,3,0,0);
    }

}

void luaPage::cleanupAndUnlink()
{
    lua_getglobal(pageLuaState,"cleanupAndUnlink");
    if(lua_isfunction(pageLuaState,-1))
    {
        lua_pcall(pageLuaState,0,0,0);
    }
    delete this;
}

luaPage::luaPage(lua_State* L)
{
    memoryTypeID = "Lua_Page";
    if(L!=NULL)
    {
        freeLuaOnDestruct = 0;
        pageLuaState = L;
    }
    else
    {
        freeLuaOnDestruct = 1;
        pageLuaState = luaL_newstate();
        luaL_openlibs(pageLuaState);
    }
}
luaPage::luaPage(lua_State* L,std::string pageDef)
{
    memoryTypeID = "Lua_Page";
    int r = 0;
    if(L!=NULL)
    {
        freeLuaOnDestruct = 0;
        pageLuaState = L;
        r = luaL_dofile(pageLuaState, pageDef.c_str());
    }
    else
    {
        freeLuaOnDestruct = 1;
        pageLuaState = luaL_newstate();
        luaL_openlibs(pageLuaState);
        r = luaL_dofile(pageLuaState, pageDef.c_str());
    }
    lua_handle_page_error(L, r);
}
luaPage::~luaPage()
{
    if(freeLuaOnDestruct)
    {
        lua_close(pageLuaState);
    }
}


/*
funct1()
{
    std::string S = "a = 7+11";
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int r = luaL_dostring(L, S.c_str());

    if(r == LUA_OK)
    {
        lua_getglobal(L,"a");
        if(lua_isnumber(L, -1))
        {
            int a = (int)lua_tonumber(L,-1);
        }
    }
    else
    {
        std::string Err = lua_tostring(L,-1);
        printf("LuaERROR: %s\n",Err.c_str());
    }
    lua_close(L);
}
*/
