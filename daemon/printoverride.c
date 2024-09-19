
#include "printoverride.h"
using namespace daemonpp;

static int l_my_print(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
			std::string s1 ("");
			dlog::info( s1 + lua_tostring(L,i));
        }
        else {
        /* Do something with non-strings if you like */
        }
    }

    return 0;
}

static const struct luaL_Reg printlib [] = {
  {"print", l_my_print},
  {NULL, NULL} /* end of array */
};

int luaopen_luamylib(lua_State *L)
{
  lua_getglobal(L, "_G");
  luaL_setfuncs(L, printlib, 0);
  lua_pop(L, 1);
}