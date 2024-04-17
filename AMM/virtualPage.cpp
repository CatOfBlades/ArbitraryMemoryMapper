
#include "virtualPage.h"
#include <cstring>

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
    memcpy( (void*)buffer, (void*)&_pageContent[offset], (std::size_t)len );

}

void virtualPage::writeMem(unsigned long int offset, unsigned char* buffer, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("virtualPage::writeMem  offset:%lu,len:%lu\n", offset, len);
    #endif
    memcpy( (void*)&_pageContent[offset], (void*)buffer, (std::size_t)len );
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
