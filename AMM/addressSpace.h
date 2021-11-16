
#ifndef ADDRESSSPACE_H_INCLUDED
#define ADDRESSSPACE_H_INCLUDED

class addressSpace
{
	public:
	//std::string memoryTypeID; //used to check for type, when dynamically loaded and unloaded.
	const char* memoryTypeID;
    addressSpace* _top;  //the address space at the beginning of the list.
    addressSpace* _this; //this address space.
    addressSpace* _next; //address space after this one.
    addressSpace* _last; //address space before this one.
    addressSpace* _bottom;  //the address space at the end of the list.
	//unsigned long int start_address; // Set by the memory manager to make memory searching slightly faster;

	virtual unsigned long int _size();
	virtual bool _is_free(); //returns 0 if this page is reserved/unreadable and unwritable
	virtual unsigned char* _content(); // returns an array of bytes of the same length as is returned by _size()
			//or returns NULL if either the size changed after the last call or the space is unreadable.

	virtual unsigned char readByte(unsigned long int offset);
	virtual void writeByte(unsigned long int offset,unsigned char Byt);

	virtual void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	virtual void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	virtual void cleanupAndUnlink();

	virtual void setParent(addressSpace* addrSp);
	addressSpace();
    addressSpace(addressSpace* parent,  unsigned int Size);
    virtual ~addressSpace();
    //virtual void operator delete(void * p);
};

#endif // ADDRESSSPACE_H_INCLUDED
