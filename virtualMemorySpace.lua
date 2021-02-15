


--addressSpace definition

local addressSpace = { memoryTypeID = "AddrSpac", _top, _this, _next, _last, _bottom}

function addressSpace:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
	--[[
	if(self~=nil) then
		setmetatable(o, self)
		self.__index = self
		return o
	else
		return nil
	end
	--]]
end

function addressSpace:new1(o,parent,Size)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	
	_last = parent
	_this = o
	if ((parent ~= nil) and (parent ~= 0))then
		_next = parent._next
		if ((_next == nil) or (_next == 0))then
			_bottom = o
			_search = parent._top
			while((_search ~= nil) and (_search ~= 0))do
				_search._bottom = _bottom
				_search = _search._next
			end
		else
			_bottom = parent._bottom
		end
		parent._next = _this
		_top = parent._top
	else
		_next = nil
		_bottom = nil
		_top = nil
	end
	
	return o
	--[[
	if(self~=nil) then
		setmetatable(o, self)
		self.__index = self
		return o
	else
		return nil
	end
	--]]
end

function addressSpace._size()
	return 0
end
function addressSpace._is_free()
	return 0
end
function addressSpace._content()
	return 0
end
function addressSpace.readByte(offset)
	return 0
end
function addressSpace.writeByte(offset,Byt)
	return 0
end
function addressSpace.readMem(offset,length)
	return 0
end
function addressSpace.writeMem(offset,buffer,length)
	return 0
end
function addressSpace.cleanupAndUnlink()
	return 0
end
function addressSpace.setParent(addrSp)
	_last = addrSp
	if ((addrSp ~= nill)or(addrSp ~= 0))then
		_next = addrSp._next
		if ((_next ~= nill)or(_next ~= 0))then
			_bottom = _this
			_search = addrSp._top
			while((_search ~= nil) and (_search ~= 0))do
				_search._bottom = _bottom
				_search = _search._next
			end
		else
			_bottom = addrSp._bottom
		end
		addrSp._next = _this
		_top = addrSp._top
	else
		_next = nil
		_bottom = nil
		_top = nil
	end
	return
end






--virtualPage definition (inherets from addressSpace)

local virtualPage = addressSpace:new()

function virtualPage:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
	--[[
	if(self~=nil) then
		setmetatable(o, self)
		self.__index = self
		return o
	else
		return nil
	end
	--]]
end

function virtualPage:new1(o,parent,Size)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	
	memoryTypeID = "Vrt_Page";
	_pageSize = Size
	_pageFree = 1
	_pageContent = {}
	_last = parent
	_this = o
	
	if ((parent ~= nil) and (parent ~= 0))then
		_next = parent._next
		if((_next == nil) or (_next == 0))then
			_bottom = o
			_search = parent._top
			while((_search ~= nil) and (_search ~= 0))do
				_search._bottom = _bottom
				_search = _search._next
			end
		else
			_bottom = parent._bottom
		end
		parent._next = _this
		_top = parent._top
	else
		_next = nil
		_bottom = nil
		_top = nil
	end	
	
	return o
end


virtualPage._pageSize=0
virtualPage._pageFree=false
virtualPage._pageContent=nil
function virtualPage._size()
	return _pageSize
end
function virtualPage._is_free()
	return _pageFree
end
function virtualPage._content()
	return _pageContent
end
function virtualPage.readByte(offset)
	return _pageContent[offset]
end
function virtualPage.writeByte(offset,Byt)
	_pageContent[offset] = Byt
	return
end
function virtualPage.readMem(offset,length)
	buffer = {}
	while(length > 0) do
		buffer[length] = _pageContent[offset+length]
		length = length-1
	end
	return buffer
end
function virtualPage.writeMem(offset,buffer,length)
	while(length > 0) do
		_pageContent[offset+length] = buffer[length]
		length = length-1
	end
	return buffer
end




--virtualMemorySpace definition

local virtualMemorySpace = {top=0,bottom,memoryOffset=0,memorySize=0,PageAddresses={}}

function virtualMemorySpace:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	print("New Memory Space",o)
	return o
	--[[
	if(self~=nil) then
		setmetatable(o, self)
		self.__index = self
		return o
	else
		return nil
	end
	--]]
end

function virtualMemorySpace.recalcSize()
	PageAddresses = {}
	memorySize = 0;
	if(top==nil or top==0)then
		return 0
	else
		_search = top
		while((_search ~= nil) and (_search ~= 0)) do
			table.insert(PageAddresses, memorySize)
			memorySize = memorySize + _search._size()
			_search = _search._next
		end
	end
	return 0
end

function virtualMemorySpace.setOffset(offset)
	memoryOffset=offset
	return 0
end

function virtualMemorySpace._getAddressPage(addr,offset) --offset is a pointer thats modified. needs fixing
	addr = addr-memoryOffset
	if(addr > memorySize)then
		return nil
	end
	
	_search = top
	while((_search ~= nil) and (_search ~= 0)) do
		if(addr < _search._size())then
			offset = addr
			return _search
		end
		addr = addr-_search._size()
		_search = _search._next
	end
	
	return 0
end

function virtualMemorySpace._getPageListInAddressRange(lowAddress, highAddress, pageCount)
	return pageList --needs defining
end

function virtualMemorySpace.addPage(Size)

	if((top == nil) or (top == 0)) then
		top = virtualPage:new()
		top._size = Size
		bottom = top
	else
		bottom = virtualPage:new1(bottom,Size)
	end
	recalcSize()
	return 0
end

function virtualMemorySpace.removePage()
	if((bottom == nil) or (bottom == 0))then
		return
	else
		if(bottom == top)then
			top.cleanupAndUnlink()
			top = nil
			bottom = nil
		else
			_tmp = bottom
			bottom = _tmp._last
			bottom._next = nil
			
			_search = top
			while((_search ~= nil) and (_search ~= 0)) do
				_search._bottom = bottom
				_search = _search._next
			end
			_tmp.cleanupAndUnlink()
		end
	end
	recalcSize()
	return 0
end

function addAddressSpace(AS)
	if((top == nil) or (top == 0))then
		top = AS
		bottom = top
		AS._last = nil
		AS._next = nil
		AS._top = AS
		return
	else
		_tmpaddr = bottom
		bottom = AS
		AS._last = _tmpaddr
		_tmpaddr._next = AS
		AS._next = nil
	end
	_search = top
	while((_search ~= nil) and (_search ~= 0)) do
		_search._bottom = bottom
		_search = _search._next
	end
	return
end

function virtualMemorySpace.readMem(Address, length)
	buffer {}
	
	Address = Address-memoryOffset
	totalBytesRead = 0
	
	startAddress = Address
	endAddress = Address+length
	
	while(length>totalBytesRead)do
		_search = top
		i=0
		if(startAddress>0)then
			while(startAddress>=PageAddresses[i])do
				i=i+1
				if((i>table.getn(PageAddresses)) or ((_search._next == nil) or (_search._next == 0)))then
					break
				end
				_search = _search._next
			end
		end
		PageTop = PageAddresses[i]
		offset = startAddress-PageTop
		offset_end = endAddress-PageTop
		if(offset_end+offset <= _search._size())then
			buffer = _search.readMem(offset,length-totalBytesRead)
			return buffer
		end
		bytesRead=0
		bytesRead = _search._size()-offset
		_search.readMem(offset,bytesRead)
		Address = Address-bytesRead
		totalBytesRead = totalBytesRead+bytesRead
	end
	
	return buffer
end

function virtualMemorySpace.writeMem(Address, buffer, length)
	Address = Address-memoryOffset
	totalBytesWritten = 0
	
	startAddress = Address
	endAddress = Address+length
	
	while(length>totalBytesWritten)do
		_search = top
		i=0
		if(startAddress>0)then
			while(startAddress>=PageAddresses[i])do
				i=i+1
				if((i>table.getn(PageAddresses)) or ((_search._next == nil) or (_search._next == 0)))then
					break
				end
				_search = _search._next
			end
		end
		PageTop = PageAddresses[i]
		offset = startAddress-PageTop
		offset_end = endAddress-PageTop
		if(offset_end+offset <= _search._size())then
			_search.writeMem(offset,buffer,length)
			return totalBytesWritten
		end
		bytesWritten=0
		bytesWritten = _search._size()-offset
		_search.writeMem(offset,buffer,bytesWritten)
		Address = Address-bytesWritten
		totalBytesWritten = totalBytesWritten + bytesWritten
		
	end
	
	return totalBytesWritten
end

function virtualMemorySpace.pageCount()
	if((top == nil)or(top==0))then
		return 0
	end
	pageCount=0
	_search = top
	while((_search ~= nil) and (_search ~= 0)) do
		pageCount = pageCount+1
		_search = _search._next
	end
	return pageCount
end

function virtualMemorySpace.addressRange(lowMem,highMem)
	if((top == nil)or(top==0))then
		highMem = memoryOffset;
		return 0
	end
	lowMem = memoryOffset;
	byteCount = 0
	_search = top
	while((_search ~= nil) and (_search ~= 0)) do
		byteCount = byteCount+_search._size()
		_search = _search._next
	end
	highMem = byteCount+memoryOffset
	return 0
end





--debug code 
VMS = virtualMemorySpace:new()
VMS.recalcSize()


