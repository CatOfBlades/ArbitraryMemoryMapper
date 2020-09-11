
--[[
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
    readMemFromContext(string ContextID,int Address,int length,char* buf)
    writeMemToContext(string ContextID,int Address,int length,char* buf)
--]]

createMemoryContext("mem1")
--Sleep(100)
addVirtualPage("page1",256)
addVirtualPage("page2",256)
addMultiPage("page3",2,{"page1","page2"})
--Sleep(100)
linkPageToMemorySpace("mem1","page1")
linkPageToMemorySpace("mem1","page2")

linkPageToMemorySpace("mem1","page3") --multipage!

--SysBeep(600,200)
i = 12
j = {}

--j[1] = i
str = "helloWorld"

leng = 0;
for itr = 1, #str do
    local c = str:sub(itr,itr)
	leng = leng+1
	j[itr] = string.byte(str,itr);
	--print(c..string.byte(str,itr))
	--print(str.byte(itr))

    -- do something with c
end

writeMemToContext("mem1",0,leng,j)

--SysBeep(500,200)
--i=10
--Sleep(500);
i = readMemFromContext("mem1",0,10)
print(string.char(i[1])..string.char(i[2])..string.char(i[3])..string.char(i[4])..string.char(i[5])..string.char(i[6])..string.char(i[7])..string.char(i[8])..string.char(i[9])..string.char(i[10]))

--SysBeep(500,200)
--Sleep(100);

--[[
while i>0 do
	SysBeep(400,200)
	--print(i)
	--print(j)
	--print(readMemFromContext("mem1",0,2))
	--print("hi")
	--Sleep(1)
	i = i-1
end
--]]

--Sleep(500);
--destroyPage("page1")
destroyMemoryContext("mem1")