

--[[--
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
--]]--

--[[--
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
--]]--



local filenumber = 0

local fname -- = "/tmp/AMM/Page"

fname = (fname .. filenumber)

local page_size = 4096

-- Ensure the file exists and is the correct size
local function ensure_file()
    local file = io.open(fname, "rb")
    if not file then
        file = io.open(fname, "wb")
        file:write(string.rep("\0", page_size))
    else
        local content = file:read("*a")
        if #content < page_size then
            file:close()
            file = io.open(fname, "ab")
            file:write(string.rep("\0", page_size - #content))
        end
    end
    file:close()
end

ensure_file()

function _size()
    return page_size
end

function _is_free()
    return 0 -- Not free
end

function readMem(offset, length)
    local f = io.open(fname, "rb")
    f:seek("set", offset)
    local data = f:read(length)
    f:close()

    local result = {}
    for i = 1, #data do
        result[i] = string.byte(data, i)
    end
    return result
end

function writeMem(offset, byt, length)
    local f = io.open(fname, "r+b")
    f:seek("set", offset)
    for i = 1, length do
        f:write(string.char(byt[i]))
    end
    f:close()
end

function cleanupAndUnlink()
    os.remove(fname)
end

function on_create()
	SysBeep(400,100)
end

function on_destroy()
	SysBeep(300,110)
end
