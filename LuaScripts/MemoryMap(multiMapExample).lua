
--[[
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
    readMemFromContext(string ContextID,int Address,int length,char* buf)
    writeMemToContext(string ContextID,int Address,int length,char* buf)
	multiPageSwapBanks(string ID, int bankNum)
--]]

createMemoryContext("mem1")
addVirtualPage("page1",10)
addVirtualPage("page2",10)
addMultiPage("page3",2,{"page1","page2"})
addVirtualPage("page4",1)
--linkPageToMemorySpace("mem1","page1")
--linkPageToMemorySpace("mem1","page2")
linkPageToMemorySpace("mem1","page3") --multipage!
linkPageToMemorySpace("mem1","page4")
	--Apperently we cannot read 10 bytes from a 10 byte memory space.
	--So we have to add 1 byte to the end here to get this working.
	--Bugfixing!

print("Hello World!")

multiPageSwapBanks("page3", 0)
j = {}
j[1] = 1
j[2] = 2
j[3] = 3
j[4] = 4
j[5] = 5
j[6] = 6
j[7] = 7
j[8] = 8
j[9] = 9
j[10] = 10
writeMemToContext("mem1",0,10,j)
multiPageSwapBanks("page3", 1)
j[1] = 10
j[2] = 9
j[3] = 8
j[4] = 7
j[5] = 6
j[6] = 5
j[7] = 4
j[8] = 3
j[9] = 2
j[10] = 1
writeMemToContext("mem1",0,10,j)
multiPageSwapBanks("page3", 0)
i = readMemFromContext("mem1",0,10)
multiPageSwapBanks("page3", 1)
k = readMemFromContext("mem1",0,10)
print(i[1]..i[2]..i[3]..i[4]..i[5]..i[6]..i[7]..i[8]..i[9]..i[10])
print(k[1]..k[2]..k[3]..k[4]..k[5]..k[6]..k[7]..k[8]..k[9]..k[10])

destroyMemoryContext("mem1")