#ifndef CALLBACKONWRITEPAGE_H_INCLUDED
#define CALLBACKONWRITEPAGE_H_INCLUDED

#include "virtualMemorySpace.h"
#include "addressSpace.h"

typedef void (*pageCallback)(virtualMemorySpace* vm,addressSpace* addrSp,unsigned long int ofss,unsigned char* buffer, unsigned long int len);

class callbackPage: public addressSpace
{
    pageCallback callback;
    virtualMemorySpace* vm;
    addressSpace* addrSp;
public:

    int pagesize; // defaults to one byte.

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();

	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();

	void setParent(addressSpace* addrSp);

	callbackPage(addressSpace* parent,pageCallback callbackfunc);
	callbackPage(addressSpace* parent,pageCallback callbackfunc,unsigned int sz);
    callbackPage(addressSpace* parent,pageCallback callbackfunc,addressSpace* adrSp);
    callbackPage(addressSpace* parent,pageCallback callbackfunc,virtualMemorySpace* vmsp);
    callbackPage(addressSpace* parent,pageCallback callbackfunc,virtualMemorySpace* vmsp,addressSpace* adrSp,unsigned int sz);
	~callbackPage();

};

#endif // CALLBACKONWRITEPAGE_H_INCLUDED
