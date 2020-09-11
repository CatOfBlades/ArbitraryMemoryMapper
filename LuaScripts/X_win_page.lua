
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

--[[
Callback Functions:

_size
_is_free
readByte
writeByte
readMem
writeMem
cleanupAndUnlink
setParent
]]--

windowTitle = "X_win2"
memoryAddress = 0xDFF5A5DE --0xdff5a5de
memoryLength = 2

--addressLow = 0xa5de;

--SysBeep(500,100)
--print("addressLow:"..string.format("%x", addressLow))--memoryAddress)
--print("memoryAddress:"..string.format("%x", memoryAddress))--memoryAddress)
--Sleep(4000)

createMemoryContext("X_win_Mem0")
addInterprocessPage("X_win_Page0", memoryLength,windowTitle,memoryAddress);
linkPageToMemorySpace("X_win_Mem0","X_win_Page0")

--Sleep(2000)

function _is_free()
	return 1
end

function _size()
	return 100
end

function readMem(offset,length)
	--SysBeep(500,100)
	--[[
	print("ReadMem( offset:"..offset..", length:"..length.." )")
	i = {}
	j = 1
	while j<=length do
		i[j] = 5
		j = j+1
	end
	return i
	--]]
	val = readMemFromContext("X_win_Mem0",offset,length)
	return val
end
function writeMem(offset,byt,length)
	--print("X_mem WriteMem( offset:"..offset..", length:"..length.." )")
	val = readMemFromContext("X_win_Mem0",offset,length)
	print( val[1] )
	--writeMemToContext("X_win_Mem0",offset,length,byt)
	
	Sleep(2000)
	--[[
	print("writeMem")
	print("offset:"..offset)
	--print("byte:"..byt)
	vardump(byt)
	print("length:"..length)
	]]--
	return
end
