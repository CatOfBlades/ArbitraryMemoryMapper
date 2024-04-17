
#ifndef ADDRESSSPACE_H_INCLUDED
#define ADDRESSSPACE_H_INCLUDED

#include "../config.h"
#include "../Defines.h"

#include <string>
#include <memory>

class addressSpace
{
	public:

	std::string memoryTypeID;
	bool reserved; //set to true to make reads only return zeros and writes fail silently
				   //You might do this if a memoryspace is mirroring system memory and
				   //a page is critical to system integrity.
	virtual unsigned long int _size();
	virtual unsigned char* _content(); // returns an array of bytes of the same length as is returned by _size()
			//or returns NULL if either the size changed after the last call or the space is unreadable.

	virtual unsigned char readByte(unsigned long int offset);
	virtual void writeByte(unsigned long int offset,unsigned char Byt);

	virtual void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	virtual void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	addressSpace();
    addressSpace(unsigned int Size);
    virtual ~addressSpace();
};

#endif // ADDRESSSPACE_H_INCLUDED
