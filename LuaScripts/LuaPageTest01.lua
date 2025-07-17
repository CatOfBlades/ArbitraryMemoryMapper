
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
	addLoggedPage(string ID, string logfile, string pageID)
    linkPageToMemorySpace(string MemorySpaceName,string PageName)
    destroyPage(string ID)
    char* buf = readMemFromContext(string ContextID,int Address,int length)
    writeMemToContext(string ContextID,int Address,int length,char* buf)
	multiPageSwapBanks(string ID, int bankNum)
--]]

--[[
Page Callback Functions:

on_create
on_destroy
_size
_is_free
readByte
writeByte
readMem
writeMem
cleanupAndUnlink
]]--

function on_create()
	SysBeep(400,100)
end

function on_destroy()
	SysBeep(300,110)
end

function _size()
	return 10
end
function _is_free()
	return 1
end
function readMem(offset,length)
	print("ReadMem( offset:"..offset..", length:"..length.." )")
	i = {}
	j = 1
	while j<=length do
		i[j] = 5
		j = j+1
	end
	return i
end
function writeMem(offset,byt,length)
	print("writeMem")
	print("offset:"..offset)
	print("length:"..length)
	--print("byte:"..byt)
	vardump(byt)
	return
end

--SysBeep(400,100)