
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

-- Initialize memory space and perform data tests
local function testMemoryFunctions()

	local memID = "mem1"
    print("test memory functions")
    createMemoryContext(memID)
    addVirtualPage("page1", 256)
    addVirtualPage("page2", 256)
	addMultiPage("page3",2,{"page1","page2"})
    linkPageToMemorySpace(memID, "page3")
    linkPageToMemorySpace(memID, "page1")
    linkPageToMemorySpace(memID, "page2")
    print("memory space initialized for testing")
    SysBeep(600, 200)

    -- Data write test
    print("preforming data write test")
    local length = 10
    local j = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    multiPageSwapBanks("page3", 0)
    print("writing 1 to 10 in bank 1")
    writeMemToContext(memID, 0, length, j)
    multiPageSwapBanks("page3", 1)
    print("writing 10 to 1 in bank 2")
    j = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}
    writeMemToContext(memID, 0, length, j)
    print("data written ready for read test")

    -- Data read test
    multiPageSwapBanks("page3", 0)
    print("read bank 1 (should be 1 to 10)")
    local k = readMemFromContext(memID, 0, length)
    for i = 1, length do
        print(k[i])
    end

    multiPageSwapBanks("page3", 1)
    print("read bank 2 (should be 10 to 1)")
    k = readMemFromContext(memID, 0, length)
    for i = 1, length do
        print(k[i])
    end

    -- Clean up resources
    destroyPage("page1")
    destroyPage("page2")
    destroyMemoryContext(memID)
end


local function testWriteAcrossConsecutivePages()

	local memID = "mem1"
    print("test split write")
    createMemoryContext(memID)
    addVirtualPage("page1", 5)
    addVirtualPage("page2", 5)
	addVirtualPage("page3", 5)
    linkPageToMemorySpace(memID, "page1")
    linkPageToMemorySpace(memID, "page2")
	linkPageToMemorySpace(memID, "page3")
    print("memory space initialized for testing")
    SysBeep(600, 200)

    -- Data write test
    print("preforming data write test")
    local length = 15
    local j = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
    print("writing 1 to 15")
    writeMemToContext(memID, 0, length, j)
    print("data written ready for read test")

    -- Data read test
    print("reading (should be 1 to 15)")
    local k = readMemFromContext(memID, 0, length)
    for i = 1, length do
        print(k[i])
    end

    -- Clean up resources
    destroyPage("page1")
    destroyPage("page2")
	destroyPage("page3")
    destroyMemoryContext(memID)
end

-- Test function for addLoggedPage
local function testAddLoggedPage()

    local memID = "mem2"
    local pageID = "log_page"
	local pageID2 = "page1"
    local logfile = "test_log.txt"
	local data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}
    
    print("Testing addLoggedPage function")

    -- Create memory context
    createMemoryContext(memID)

	-- Add virtual memory for logged page to refrence
	addVirtualPage(pageID2, #data)
    -- Add a logged page
    addLoggedPage(pageID, logfile, pageID2)

    -- Link to memory space
    linkPageToMemorySpace(memID, pageID)

    -- Write some data to the logged page
	print("writing")
    writeMemToContext(memID, 0, #data, data )

    -- Data read test
    print("reading")
    local k = readMemFromContext(memID, 0, #data)
    for i = 1, #data do
        print(k[i])
    end

    -- Clean up resources
	destroyPage(pageID2)
    destroyPage(pageID)
    destroyMemoryContext(memID)

    print("addLoggedPage function test completed")
end

-- Test function for addMetaPage
local function testMetaPage()
	--addMetaPage(string ID, int PageSize,string SubMemorySpaceName,int address)

    local memID = "mem1"
	local memID2 = "mem2"
    local pageID1 = "page1"
	local pageID2 = "page2"
	local metapage1 = "meta1"
	local metapage2 = "meta2"

	local data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
    
    print("Testing addMetaPage function")

    -- Create memory context
    createMemoryContext(memID)
	createMemoryContext(memID2)

	-- Add virtual memory for meta page to refrence
	-- only allocating 10 bytes even though data is 15 bytes
	addVirtualPage(pageID1, 4) -- uneven to test if mirror can read across multiple pages
	addVirtualPage(pageID2, 6)
	
    -- Add a meta page
	-- metaPage mirrors the first five bytes giving the illusion of more memory
	addMetaPage(metapage2, 5,memID,0)
	
	addMetaPage(metapage1, 14,memID,1) --one offset to show that works


    -- Link to memory space
    linkPageToMemorySpace(memID, pageID1)
	linkPageToMemorySpace(memID, pageID2)
	linkPageToMemorySpace(memID, metapage2)
	--first memory space has three pages totaling an addressspace of 15 bytes
	
	linkPageToMemorySpace(memID2, metapage1)
	--seccond memory space has 14 bytes mirroring the last 14 bytes of memoryspace

    -- Write some data to the logged page
	print("writing")
    writeMemToContext(memID, 0, #data, data )

    -- Data read test
    print("reading memorySpace1 (should be 11, 12, 13, 14, 15, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)")
    local k = readMemFromContext(memID, 0, #data)
    for i = 1, #data do
        print(k[i])
    end
    print("reading memorySpace2 (should be 12, 13, 14, 15, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)")
    local k = readMemFromContext(memID2, 0, #data-1)
    for i = 1, #data-1 do
        print(k[i])
    end

	-- Data mirror test
	print("writing 0, 1, 3, 5, 7, 9, 12, 15, 32, 47, 8, 76, 44, 7 to metaspace")
	data = {0, 1, 3, 5, 7, 9, 12, 15, 32, 47, 8, 76, 44, 7}
	writeMemToContext(memID2, 0, #data, data )
    print("reading memorySpace1 (47, 8, 76, 44, 7, 7, 9, 12, 15, 32, 47, 8, 76, 44)")
    local k = readMemFromContext(memID, 0, #data)
    for i = 1, #data do
        print(k[i])
    end

    -- Clean up resources
    destroyPage(pageID1)
	destroyPage(pageID2)
	destroyPage(metapage1)
	destroyPage(metapage2)
    destroyMemoryContext(memID)
	destroyMemoryContext(memID2)

    print("addMetaPage function test completed")
end

-- Run tests
testMemoryFunctions()
testWriteAcrossConsecutivePages()
testAddLoggedPage()
testMetaPage()
