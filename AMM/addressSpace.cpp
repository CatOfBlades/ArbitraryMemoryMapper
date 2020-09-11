#include "addressSpace.h"

/** addressSpace VTABLE declarations **/

unsigned long int addressSpace::_size()
{
    return 0;
}
/*
bool addressSpace::_is_free(){}; //returns 0 if this page is reserved/unreadable and unwritable
unsigned char* addressSpace::_content(){};
unsigned char addressSpace::readByte(unsigned long int offset){};
void addressSpace::writeByte(unsigned long int offset,unsigned char Byt){};
void addressSpace::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len){};
void addressSpace::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len){};

*/
void addressSpace::cleanupAndUnlink()
{
    delete this;
    return;
}

void addressSpace::setParent(addressSpace* addrSp)
{
    _last = addrSp;
    if(addrSp)
    {
        _next = addrSp->_next;
        if(_next == nullptr)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = addrSp->_top;
            while(_search != nullptr)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = addrSp->_bottom;
        }
        addrSp->_next = _this;
        _top = addrSp->_top;
    }
    else
    {
        _next = nullptr;
        _bottom = nullptr;
        _top = nullptr;
    }
}
addressSpace::addressSpace()
{
    memoryTypeID = "AddrSpac";
    _this = this;
    _next = nullptr;
    _last = nullptr;
    _top = nullptr;
    _bottom = nullptr;
}
addressSpace::addressSpace(addressSpace* parent,  unsigned int Size)
{
    memoryTypeID = "AddrSpac";
    _last = parent;
    _this = this;
    if(parent)
    {
        _next = parent->_next;
        if(_next == nullptr)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = parent->_top;
            while(_search != nullptr)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = parent->_bottom;
        }
        parent->_next = _this;
        _top = parent->_top;
    }
    else
    {
        _next = nullptr;
        _bottom = nullptr;
        _top = nullptr;
    }
}

addressSpace::~addressSpace()
{
    return;
}

bool addressSpace::_is_free()
{
    return 0;
}

unsigned char* addressSpace::_content()
{
    return 0;
}

unsigned char addressSpace::readByte(unsigned long int offset)
{
    return 0;
}
void addressSpace::writeByte(unsigned long int offset,unsigned char Byt){};

void addressSpace::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len){};
void addressSpace::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len){};
