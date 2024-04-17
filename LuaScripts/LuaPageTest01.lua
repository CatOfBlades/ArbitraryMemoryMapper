
require "vardump"

--[[
Callback Functions:

_size
_is_free
readByte
writeByte
readMem
writeMem
cleanupAndUnlink
]]--

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