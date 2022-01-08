
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
        //printf("offset:%i,len:%i\n",offset,len);
        buffer[len-1] = _pageContent[offset+len-1];
        len -= 1;
    }
    return;
}
void virtualPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    while(len > 0)
    {
        #ifdef EXTRA_DEBUG_MESSAGES
        printf("virtualPage::writeMem  offset:%i,len:%i\n",offset,len);
        #endif
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

    initialLink(parent,Size);

#ifdef BUILT_IN_VISUALIZER
    VisWM = visualizePage(this);
#endif // BUILT_IN_VISUALIZER

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
