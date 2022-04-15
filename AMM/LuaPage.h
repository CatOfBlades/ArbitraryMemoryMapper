
#ifndef LUAPAGE_H_INCLUDED
#define LUAPAGE_H_INCLUDED

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}
#include "addressSpace.h"
#include <string>

class luaPage: public addressSpace
{
	public:

    bool freeLuaOnDestruct;
    lua_State* pageLuaState;

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();

	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();

	void setParent(addressSpace* addrSp);
	luaPage(lua_State* L);
    luaPage(lua_State* L,std::string pageDef);
    ~luaPage();
};


#endif // LUAPAGE_H_INCLUDED
