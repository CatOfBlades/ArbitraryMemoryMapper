
#ifndef LUAPAGE_H_INCLUDED
#define LUAPAGE_H_INCLUDED

extern "C"
{
    #include "../lua-5.4.0/src/lua.h"
    #include "../lua-5.4.0/src/lauxlib.h"
    #include "../lua-5.4.0/src/lualib.h"
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
    luaPage(lua_State* L,addressSpace* parent,std::string pageDef);
    luaPage(lua_State* L,addressSpace* parent);
    ~luaPage();
};


#endif // LUAPAGE_H_INCLUDED
