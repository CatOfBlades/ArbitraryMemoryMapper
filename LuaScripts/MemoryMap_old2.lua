
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

print("test memory functions")

createMemoryContext("mem1")
addVirtualPage("page1",256)
--addVirtualPage("page2",256)
--addMultiPage("page3",2,{"page1","page2"})
linkPageToMemorySpace("mem1","page1")
--linkPageToMemorySpace("mem1","page2")
--linkPageToMemorySpace("mem1","page3") --linking the multipage as the first page to show off memory swapping potential

print("memory space initialized for testing")
SysBeep(600,200)

print("preforming data write test")
length = 10
i = 1
j = {}

while i<=length do
	j[i] = i
	i=i+1
end
--multiPageSwapBanks("page3", 0) --bank numbers start at 0 so it is the first bank and 1 is the seccond bank
writeMemToContext("mem1",0,length,j)
--multiPageSwapBanks("page3", 1)
while i>0 do
	j[length-i+1] = i
	i=i-1
end
--writeMemToContext("mem1",0,length,j)
print("data writen ready for read test")

--multiPageSwapBanks("page3", 0) --swapping back to bank 0 (page1)
k = readMemFromContext("mem1",0,length) --note that we are reading from address '0' this is the "top" of our memory space.

--printing the contents of bank 0
i=0
while i<=length do
	print(k[i])
	i=i+1
end

--multiPageSwapBanks("page3", 1) --swapping to bank 1 (page2)
--k = readMemFromContext("mem1",0,length)

--printing the contents of bank 1 to show they are different.
--i=0
--while i<=length do
--	print(k[i])
--	i=i+1
--end

destroyPage("page1") --we have to clean these up manually because multipages don't destroy pages they refrence.
--destroyPage("page2") --note: destroying a page frees it from the memoryContext that its linked to. So you can replace it by linking another.
destroyMemoryContext("mem1") --page3 is destroyed by the memoryContext when it dies because they are linked.
