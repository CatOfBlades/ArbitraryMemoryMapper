#ifndef VIRTUALPAGE_H_INCLUDED
#define VIRTUALPAGE_H_INCLUDED

#include "addressSpace.h"

class virtualPage: public addressSpace
{
    public:
	unsigned long int _pageSize;
	bool _pageFree;
	unsigned char* _pageContent;

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	//void setParent(addressSpace* addrSp);

	virtualPage();
	virtualPage(unsigned int Size);
	~virtualPage();
};

#endif // VIRTUALPAGE_H_INCLUDED
