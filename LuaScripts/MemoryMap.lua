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
    linkPageToMemorySpace(memID, "page1")
    linkPageToMemorySpace(memID, "page2")
    print("memory space initialized for testing")
    SysBeep(600, 200)

    -- Data write test
    print("preforming data write test")
    local length = 10
    local j = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    print("writing 1 to 10")
    writeMemToContext(memID, 0, length, j)
    print("data written ready for read test")

    -- Data read test
    print("reading (should be 1 to 10)")
    local k = readMemFromContext(memID, 0, length)
    for i = 1, length do
        print(k[i])
    end

    -- Clean up resources
    destroyPage("page1")
    destroyPage("page2")
    destroyMemoryContext(memID)
end

-- Test function for addLoggedPage
local function testAddLoggedPage()

    local memID = "mem2"
    local pageID = "log_page"
	local pageID2 = "page1"
    local logfile = "test_log.txt"
	local data = "This is a test log message."
    
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
    writeMemToContext(memID, 0, 27, data)

    -- Data read test
    print("reading")
    local k = readMemFromContext(memID, 0, 27)
    for i = 1, #data do
        print(k[i])
    end

    -- Clean up resources
	destroyPage(pageID2)
    destroyPage(pageID)
    destroyMemoryContext(memID)

    print("addLoggedPage function test completed")
end

-- Run tests
testMemoryFunctions()
testWriteAcrossConsecutivePages()
testAddLoggedPage()
