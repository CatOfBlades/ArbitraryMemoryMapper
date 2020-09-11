
#include "virtualPage.h"

unsigned long int virtualPage::_size()
{
    return _pageSize;
}
bool virtualPage::_is_free()
{
    return _pageFree;
}
unsigned char* virtualPage::_content()
{
    return _pageContent;
}
unsigned char virtualPage::readByte(unsigned long int offset)
{
    return _pageContent[offset];
}
void virtualPage::writeByte(unsigned long int offset,unsigned char Byt)
{
    _pageContent[offset] = Byt;
}

void virtualPage::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
    while(len > 0)
    {
        buffer[len-1] = _pageContent[offset+len-1];
        len -= 1;
    }
    return;
}
void virtualPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    while(len > 0)
    {
        _pageContent[offset+len-1] = Byt[len-1];
        len -= 1;
    }
    return;
}

virtualPage::virtualPage():addressSpace()
{
    memoryTypeID = "Vrt_Page";
    _pageSize = 0;
    _pageFree = 1;
}

/*
void virtualPage::setParent(addressSpace* addrSp)
{
    _last = addrSp;
    _top = addrSp->_top;
    addrSp->_next = _this;
}
*/


#include "stdio.h"
virtualPage::virtualPage(addressSpace* parent,  unsigned int Size)
{
    memoryTypeID = "Vrt_Page";
    _pageSize = Size;
    _pageFree = 1;
    _pageContent = new unsigned char[_pageSize];

    _last = parent;
    _this = this;

    //printf("testQ1 parent: %p\n", parent);
    if(parent != nullptr)
    {
        //printf("testQ2\n");
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
//#include <windows.h>
virtualPage::~virtualPage()
{
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
    return;
}
void virtualPage::cleanupAndUnlink()
{
    delete this; // only used because the 'virtualMemorySpace' object calls 'new' to create these objects
            //so don't call 'cleanupAndUnlink()' if created in another way.
            //may update to allow for multiple memory spaces to reference the same page.
    return;
}
