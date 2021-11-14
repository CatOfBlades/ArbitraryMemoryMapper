#ifndef FILEPAGE_H_INCLUDED
#define FILEPAGE_H_INCLUDED


#include "addressSpace.h"
#include<stdio.h>
#include<string>

class filePage: public addressSpace
{
    public:
	FILE *fp;
	int cl;
	std::string FMfilename;
	long lSize;
	void FMopenFile();
	void FMcloseFile();
	unsigned long int RW_file(bool write, unsigned long int offs, unsigned char* dat, unsigned long int len);
	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();

	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	void setParent(addressSpace* addrSp);

	filePage();
    filePage(addressSpace* parent,  unsigned int Size);
    filePage(std::string filename,  unsigned int Size);
};


#endif // FILEPAGE_H_INCLUDED
