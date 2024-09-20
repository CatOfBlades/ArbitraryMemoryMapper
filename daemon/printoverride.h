
/***\
The daemon should output all lua print statements to the system logs.
This is the header for the functions that register that print statement to the lua instance.
\***/

#ifndef PRINTOVERRIDE_H
#define PRINTOVERRIDE_H

#include "../lua-5.4.4/src/lua.h"
#include "../lua-5.4.4/src/lauxlib.h"
#include "../lua-5.4.4/src/lualib.h"


static int l_my_print(lua_State* L);
int luaopen_luamylib(lua_State *L);


#endif