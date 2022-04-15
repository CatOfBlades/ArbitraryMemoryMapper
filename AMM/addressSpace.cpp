#include "addressSpace.h"


/*

unsigned long int addressSpace::_size(){};
unsigned char* addressSpace::_content(){};
unsigned char addressSpace::readByte(unsigned long int offset){};
void addressSpace::writeByte(unsigned long int offset,unsigned char Byt){};
void addressSpace::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len){};
void addressSpace::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len){};

*/

unsigned long int addressSpace::_size()
{
    return 0;
}

unsigned char* addressSpace::_content()
{
    return 0;
}

addressSpace::addressSpace()
{
	reserved = false;
}

addressSpace::~addressSpace()
{
    return;
}



unsigned char addressSpace::readByte(unsigned long int offset)
{
	unsigned char byt;
	readMem(offset,&byt, 1);
    return byt;
}
void addressSpace::writeByte(unsigned long int offset,unsigned char Byt)
{
	writeMem(offset,&Byt,1);
}

void addressSpace::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
	return;
}
void addressSpace::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
	return;
}
