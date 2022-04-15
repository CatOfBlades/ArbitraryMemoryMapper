#ifndef VIRTUALPAGE_H_INCLUDED
#define VIRTUALPAGE_H_INCLUDED

#include "addressSpace.h"

class virtualPage: public addressSpace
{
    public:
	unsigned long int _pageSize;
	unsigned char* _pageContent;

	unsigned long int _size();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	virtualPage();
	virtualPage(unsigned int Size);
	~virtualPage();
};

#endif // VIRTUALPAGE_H_INCLUDED
