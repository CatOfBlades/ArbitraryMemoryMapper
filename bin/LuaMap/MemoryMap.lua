
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
    linkPageToMemorySpace(string MemorySpaceName,string PageName)
    destroyPage(string ID)
    readMemFromContext(string ContextID,int Address,int length,char* buf)
    writeMemToContext(string ContextID,int Address,int length,char* buf)
    multiPageSwapBanks(string ID, int bankNum)
--]]

createMemoryContext("mem1")
addVirtualPage("page1",5)
addVirtualPage("page2",32)
--addMultiPage("page3",2,{"page1","page2"})
linkPageToMemorySpace("mem1","page1")
linkPageToMemorySpace("mem1","page2")
--linkPageToMemorySpace("mem1","page3") --linking the multipage as the first page to show off memory swapping potential

SysBeep(600,200)
length = 10
i = 1
j = {}

while i<=length do
	j[i] = i
	i=i+1
end
--print("hi...")
--multiPageSwapBanks("page3", 0) --bank numbers start at 0 so it is the first bank and 1 is the seccond bank
writeMemToContext("mem1",0,length,j)
--print("yes, hi.")
--multiPageSwapBanks("page3", 1)
while i>0 do
	j[length-i+1] = i
	i=i-1
end
writeMemToContext("mem1",0,length,j)

--multiPageSwapBanks("page3", 0) --swapping back to bank 0 (page1)
k = readMemFromContext("mem1",0,length) --note that we are reading from address '0' this is the "top" of our memory space.

--printing the contents of bank 0
i=1
while i<=length do
	print(k[i])
	i=i+1
end


--multiPageSwapBanks("page3", 1) --swapping to bank 1 (page2)
k = readMemFromContext("mem1",0,length)

--printing the contents of bank 1 to show they are different.
i=1
while i<=length do
	print(k[i])
	i=i+1
end


print("wraptest:") --testing the new memory wrap functionality. no longer crashing upon negitive addresses or otherwise out of bound accesses.
i=1
j[1] = 7
while i<length do
	writeMemToContext("mem1",-5+i,1,j)
	i=i+1
end

i=1
while i<length+5 do
	k = readMemFromContext("mem1",-5+i,1)
	print(k[1])
	i=i+1
end


destroyPage("page1") --we have to clean these up manually because multipages don't destroy pages they refrence.
destroyPage("page2") --note: destroying a page frees it from the memoryContext that its linked to. So you can replace it by linking another.
destroyMemoryContext("mem1") --page3 is destroyed by the memoryContext when it dies because they are linked.
