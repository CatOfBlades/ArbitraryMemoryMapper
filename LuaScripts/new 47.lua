

--[[
    createMemoryContext(string ID)
    destroyMemoryContext(string ID)
    SysBeep(int freq,int time)
    Sleep(int time)
    addVirtualPage(string ID, int PageSize)
    addInterprocessPage(string ID, int PageSize,string windowName,int address)
    addMultiPage(string ID, int PageSize, int pagelistSize, string** pagelist)
    addMetaPage(string ID, int PageSize,string SubMemorySpaceName,int address)
    addLuaPage(string ID, string luafile)
	addLoggedPage(string ID, string logfile, string pageID)
    linkPageToMemorySpace(string MemorySpaceName,string PageName)
    destroyPage(string ID)
    readMemFromContext(string ContextID,int Address,int length,char* buf)
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


varlist = { {name="value1",value=10},{name="value2",value=0}  }

