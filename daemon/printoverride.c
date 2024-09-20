
#include "printoverride.h"

static int l_my_print(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
			size_t len = 0;
			char* txt = lua_tolstring(L,i,&len);
			
			printf("%s",txt);
			FILE * log = fopen("/var/log/luamapd","w+");
			fwrite(txt, len, 1, log);
			fclose(log);
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