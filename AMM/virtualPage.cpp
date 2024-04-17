
#include "virtualPage.h"

unsigned long int virtualPage::_size()
{
    return _pageSize;
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

void virtualPage::readMem(unsigned long int offset, unsigned char* buffer, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("virtualPage::readMem  offset:%lu,len:%lu\n", offset, len);
    #endif
    for (unsigned long int i = 0; i < len; ++i)
    {
        buffer[i] = _pageContent[offset + i];
    }
}

void virtualPage::writeMem(unsigned long int offset, unsigned char* Byt, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("virtualPage::writeMem  offset:%lu,len:%lu\n", offset, len);
    #endif
    for (unsigned long int i = 0; i < len; ++i)
    {
        _pageContent[offset + i] = Byt[i];
    }
}


virtualPage::virtualPage():addressSpace()
{
    memoryTypeID = "Vrt_Page";
    _pageSize = 0;
}


#include "stdio.h"
virtualPage::virtualPage( unsigned int Size)
{
    memoryTypeID = "Vrt_Page";
    _pageSize = Size;
    _pageContent = new unsigned char[_pageSize];

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
