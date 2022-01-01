
#include "selfRefrence.h"

unsigned long int memorySpacePage::_size()
{
    return _pageSize;
}
bool memorySpacePage::_is_free()
{
    if(_pageSize>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
unsigned char* memorySpacePage::_content()
{
    if(_pageSize<=0)
    {
        return NULL;
    }
    readMem(0,_pageContent,_pageSize);
    return _pageContent;
}
unsigned char memorySpacePage::readByte(unsigned long int offset)
{
    unsigned char C;
    childVM->readMem(_address+offset,&C,1);
    return C;
}
void memorySpacePage::writeByte(unsigned long int offset,unsigned char Byt)
{
    childVM->writeMem(_address+offset,&Byt,1);
    return;
}

void memorySpacePage::readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len)
{
    childVM->readMem(_address+offset,buffer,len);
    return;
}
void memorySpacePage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    childVM->writeMem(_address+offset,Byt,len);
    return;
}

void memorySpacePage::cleanupAndUnlink()
{
    delete this;
    return;
}
void memorySpacePage::linkToVM(virtualMemorySpace* VM,unsigned long int address, unsigned int Size)
{
    childVM = VM;
    _address = address;
    _pageSize = Size;
}
memorySpacePage::memorySpacePage()
{
    memoryTypeID = "MemSpPag";
    childVM = NULL;
    _address = 0;
    _pageSize = 0;
}
memorySpacePage::memorySpacePage(virtualMemorySpace* VM,unsigned long int address, unsigned int Size)
{
    memoryTypeID = "MemSpPag";
    childVM = VM;
    _address = address;
    _pageSize = Size;
    _pageContent = new unsigned char[_pageSize];

    _this = this;
    _next = NULL;
    _last = NULL;
    _top = NULL;
    _bottom = NULL;
}
memorySpacePage::memorySpacePage(virtualMemorySpace* VM,char* address, unsigned int Size)
{
    memoryTypeID = "MemSpPag";
    childVM = VM;
    _paddress = address;
    _pageSize = Size;
    _pageContent = new unsigned char[_pageSize];

    _this = this;
    _next = NULL;
    _last = NULL;
    _top = NULL;
    _bottom = NULL;
}
memorySpacePage::memorySpacePage(addressSpace* parent, unsigned int Size)
{
    memoryTypeID = "MemSpPag";
    _pageSize = Size;
    _pageContent = new unsigned char[_pageSize];

    initialLink(parent,Size);
}
memorySpacePage::~memorySpacePage()
{
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
}
