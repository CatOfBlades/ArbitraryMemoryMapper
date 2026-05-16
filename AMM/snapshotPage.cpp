
#include "snapshotPage.h"
#include <cstring>

unsigned long int snapshotPage::_size()
{
    return _pageSize;
}
unsigned char* snapshotPage::_content()
{
    return _pageContent;
}
unsigned char snapshotPage::readByte(unsigned long int offset)
{
    return _pageContent[offset];
}
void snapshotPage::writeByte(unsigned long int offset,unsigned char Byt)
{
	/** writing to a snapshot would invalidate the snapshot so it is disallowed **/
    //_pageContent[offset] = Byt;
	#ifdef DBG_MESSAGES
		printf("Warning: Attempt to write to snapshot page is disallowed, no memory written!\n");
    #endif
}

void snapshotPage::readMem(unsigned long int offset, unsigned char* buffer, unsigned long int len)
{
    #ifdef EXTRA_DEBUG_MESSAGES
    printf("readMem snapshotPage:%p offset:%lu,len:%lu\n", this, offset, len);
    #endif
    memcpy( (void*)buffer, (void*)&_pageContent[offset], (std::size_t)len );

}

void snapshotPage::writeMem(unsigned long int offset, unsigned char* buffer, unsigned long int len)
{

	/** writing to a snapshot would invalidate the snapshot so it is disallowed **/
	//memcpy( (void*)&_pageContent[offset], (void*)buffer, (std::size_t)len );
	#ifdef EXTRA_DEBUG_MESSAGES
		printf("writeMem snapshotPage:%p  offset:%lu,len:%lu\n", this, offset, len);
    #endif
	#ifdef DBG_MESSAGES
	printf("Warning: Attempt to write to snapshot page is disallowed, no memory written!\n");
	#endif
}


snapshotPage::snapshotPage():addressSpace()
{
    memoryTypeID = "Snp_Page";
    _pageSize = 0;
}


#include "stdio.h"
snapshotPage::snapshotPage( std::shared_ptr<addressSpace> AS )
{
	Link(AS);
    memoryTypeID = "Snp_Page";
    _pageSize = AS->_size();
    _pageContent = new unsigned char[_pageSize];
	updateSnapshot();

}
//#include <windows.h>
snapshotPage::~snapshotPage()
{
    if(_pageSize > 0)
    {
        delete [] _pageContent;
    }
    return;
}

void snapshotPage::updateSnapshot()
{
	int i = 0;
	while(i < _pageSize)
	{
		_pageContent[i] = linkedPage->readByte(i);
	}
}

void snapshotPage::Link(std::shared_ptr<addressSpace> AS)
{
	linkedPage = AS;
}

void snapshotPage::cleanupAndUnlink()
{
	linkedPage = nullptr;
    delete this;
    return;
}