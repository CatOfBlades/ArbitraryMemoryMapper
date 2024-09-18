cmake_minimum_required(VERSION 3.10)

# Specify the source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")  # Adjust as needed


add_library(LuaLib STATIC
lapi.c
lauxlib.c
lbaselib.c
lcode.c
lcorolib.c
lctype.c
ldblib.c
ldebug.c
ldo.c
ldump.c
lfunc.c
lgc.c
linit.c
liolib.c
llex.c
lmathlib.c
lmem.c
loadlib.c
lobject.c
lopcodes.c
loslib.c
lparser.c
lstate.c
lstring.c
lstrlib.c
ltable.c
ltablib.c
ltm.c
lundump.c
lutf8lib.c
lvm.c
lzio.c
)

set_target_properties(LuaLib PROPERTIES LINKER_LANGUAGE C)

target_include_directories(LuaLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
