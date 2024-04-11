

CPP      = g++
CC		= gcc
CFLAGS  = -c -DLINUX_BUILD -static -static-libgcc -static-libstdc++
INCLUDES = -I ./lua-5.4.4/src/#,/AMM/
RM      = rm -f

BINDIR = ./bin
AMMDIR = ./AMM
LUADIR = ./lua-5.4.4/src

#Comment this out if you are linking a pre-built lua
COMPILE_AND_LINK_LUA =1 

default: all

all: LuaMap
    
LuaMap: LuaMemoryMapper.cpp
	make clean
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/InterprocessMemorySpace.o $(AMMDIR)/InterprocessMemorySpace.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/LoggedPage.o $(AMMDIR)/LoggedPage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/LuaPage.o $(AMMDIR)/LuaPage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/WindowsPageAccessor.o $(AMMDIR)/WindowsPageAccessor.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/addressSpace.o $(AMMDIR)/addressSpace.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/callbackOnWritePage.o $(AMMDIR)/callbackOnWritePage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/filePage.o $(AMMDIR)/filePage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/math/hsv2rgb.o $(AMMDIR)/math/hsv2rgb.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/multiPage.o $(AMMDIR)/multiPage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/pageVisualizer.o $(AMMDIR)/pageVisualizer.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/selfRefrence.o $(AMMDIR)/selfRefrence.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/virtualMemorySpace.o $(AMMDIR)/virtualMemorySpace.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(AMMDIR)/virtualPage.o $(AMMDIR)/virtualPage.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o ArbitraryMemMap.o ArbitraryMemMap.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o LuaMemoryMapper.o LuaMemoryMapper.cpp
	$(CPP) $(CFLAGS) $(INCLUDES) -o LuaMemoryMapperLib.o LuaMemoryMapperLib.cpp

	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lapi.o $(LUADIR)/lapi.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lauxlib.o $(LUADIR)/lauxlib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lbaselib.o $(LUADIR)/lbaselib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lcode.o $(LUADIR)/lcode.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lcorolib.o $(LUADIR)/lcorolib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lctype.o $(LUADIR)/lctype.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/ldblib.o $(LUADIR)/ldblib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/ldebug.o $(LUADIR)/ldebug.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/ldo.o $(LUADIR)/ldo.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/ldump.o $(LUADIR)/ldump.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lfunc.o $(LUADIR)/lfunc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lgc.o $(LUADIR)/lgc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/linit.o $(LUADIR)/linit.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/liolib.o $(LUADIR)/liolib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/llex.o $(LUADIR)/llex.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lmathlib.o $(LUADIR)/lmathlib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lmem.o $(LUADIR)/lmem.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/loadlib.o $(LUADIR)/loadlib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lobject.o $(LUADIR)/lobject.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lopcodes.o $(LUADIR)/lopcodes.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/loslib.o $(LUADIR)/loslib.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lparser.o $(LUADIR)/lparser.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lstate.o $(LUADIR)/lstate.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(LUADIR)/lstring.o $(LUADIR)/lstring.c
	
	$(CPP) -o $(BINDIR)/LuaMap/LuaMap $(LUADIR)/lapi.o $(LUADIR)/lauxlib.o $(LUADIR)/lbaselib.o $(LUADIR)/lcode.o $(LUADIR)/lcorolib.o $(LUADIR)/lctype.o $(LUADIR)/ldblib.o $(LUADIR)/ldebug.o $(LUADIR)/ldo.o $(LUADIR)/ldump.o $(LUADIR)/lfunc.o $(LUADIR)/lgc.o $(LUADIR)/linit.o $(LUADIR)/liolib.o $(LUADIR)/llex.o $(LUADIR)/lmathlib.o $(LUADIR)/lmem.o $(LUADIR)/loadlib.o $(LUADIR)/lobject.o $(LUADIR)/lopcodes.o $(LUADIR)/loslib.o $(LUADIR)/lparser.o $(LUADIR)/lstate.o $(LUADIR)/lstring.o $(AMMDIR)/InterprocessMemorySpace.o $(AMMDIR)/LoggedPage.o $(AMMDIR)/LuaPage.o $(AMMDIR)/WindowsPageAccessor.o $(AMMDIR)/addressSpace.o $(AMMDIR)/callbackOnWritePage.o $(AMMDIR)/filePage.o $(AMMDIR)/math/hsv2rgb.o $(AMMDIR)/multiPage.o $(AMMDIR)/pageVisualizer.o $(AMMDIR)/selfRefrence.o $(AMMDIR)/virtualMemorySpace.o $(AMMDIR)/virtualPage.o LuaMemoryMapper.o
	
clean LuaMapclean:
	$(RM) $(BINDIR)/LuaMap/LuaMap
	$(RM) $(AMMDIR)/InterprocessMemorySpace.o
	$(RM) $(AMMDIR)/LoggedPage.o
	$(RM) $(AMMDIR)/LuaPage.o
	$(RM) $(AMMDIR)/WindowsPageAccessor.o
	$(RM) $(AMMDIR)/addressSpace.o
	$(RM) $(AMMDIR)/callbackOnWritePage.o
	$(RM) $(AMMDIR)/filePage.o
	$(RM) $(AMMDIR)/math/hsv2rgb.o
	$(RM) $(AMMDIR)/multiPage.o
	$(RM) $(AMMDIR)/pageVisualizer.o
	$(RM) $(AMMDIR)/selfRefrence.o
	$(RM) $(AMMDIR)/virtualMemorySpace.o
	$(RM) $(AMMDIR)/virtualPage.o
	$(RM) ArbitraryMemMap.o
	$(RM) LuaMemoryMapper.o
	$(RM) LuaMemoryMapperLib.o
	$(RM) $(LUADIR)/lauxlib.o
	$(RM) $(LUADIR)/lapi.o
	$(RM) $(LUADIR)/lbaselib.o
	$(RM) $(LUADIR)/lcode.o
	$(RM) $(LUADIR)/lcorolib.o
	$(RM) $(LUADIR)/lctype.o
	$(RM) $(LUADIR)/ldblib.o
	$(RM) $(LUADIR)/ldebug.o
	$(RM) $(LUADIR)/ldo.o
	$(RM) $(LUADIR)/ldump.o
	$(RM) $(LUADIR)/lfunc.o
	$(RM) $(LUADIR)/lgc.o
	$(RM) $(LUADIR)/linit.o
	$(RM) $(LUADIR)/liolib.o
	$(RM) $(LUADIR)/llex.o
	$(RM) $(LUADIR)/lmathlib.o
	$(RM) $(LUADIR)/lmem.o
	$(RM) $(LUADIR)/loadlib.o
	$(RM) $(LUADIR)/lobject.o
	$(RM) $(LUADIR)/lopcodes.o
	$(RM) $(LUADIR)/loslib.o
	$(RM) $(LUADIR)/lparser.o
	$(RM) $(LUADIR)/lstate.o
	$(RM) $(LUADIR)/lstring.o
	