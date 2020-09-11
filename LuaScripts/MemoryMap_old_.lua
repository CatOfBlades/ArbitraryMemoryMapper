
-- "require" is apperently like #include but the file has to be a .lua file

--require "MemoryMap(multiMapExample)"
require "vardump"

--[[
	System Defined functions:

    createMemoryContext(string ID)
    destroyMemoryContext(string ID)
    SysBeep(int freq,int time)
    addVirtualPage(string ID, int PageSize)
    addInterprocessPage(string ID, int PageSize,string windowName,int address)
    addMultiPage(string ID, int PageSize, int pagelistSize, string** pagelist)
    addMetaPage(string ID, int PageSize,string SubMemorySpaceName,int address)
    addLuaPage(string ID, string luafile)
    linkPageToMemorySpace(string MemorySpaceName,string PageName)
    destroyPage(string ID)
    char* buf = readMemFromContext(string ContextID,int Address,int length)
    writeMemToContext(string ContextID,int Address,int length,char* buf)
	multiPageSwapBanks(string ID, int bankNum)
--]]

-- Note: Lua page scripts have to return before the main thread can continue
		-- so a page script that has a loop might lock your program

createMemoryContext("mem0")
addVirtualPage("page0", 64)
addLuaPage("page1","LuaPageTest01.lua")
linkPageToMemorySpace("mem0","page0")
linkPageToMemorySpace("mem0","page1")

--SysBeep(500,100)

i = {100,100,0}
v = 2000
while v>0 do
writeMemToContext("mem0",0,2,i)
Sleep(20)
--SysBeep(400,100)
v = v-1
end
--val = readMemFromContext("mem0",0,10)
--SysBeep(500,100)

--print( val[1]..":"..val[2]..":"..val[3]..":"..val[4]..":"..val[5]..":"..val[6]..":"..val[7]..":"..val[8]..":"..val[9]..":"..val[10] )

destroyMemoryContext("mem0")
