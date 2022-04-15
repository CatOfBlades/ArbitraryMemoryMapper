
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
