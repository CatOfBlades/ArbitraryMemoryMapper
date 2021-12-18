#include "callbackOnWritePage.h"


	unsigned long int callbackPage::_size()
	{
	    return pagesize;
	}
	bool callbackPage::_is_free()
	{
	    return true;
	}
	unsigned char* callbackPage::_content()
	{
	    return 0;
	}

	unsigned char callbackPage::readByte(unsigned long int offset)
	{
	    return 0;
	}
	void callbackPage::writeByte(unsigned long int offset,unsigned char Byt)
	{
	    callback(vm,addrSp,offset,&Byt,1);
	}

	void callbackPage::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
	{
	    len-=1;
	    while(len>=0)
	    {
	        buffer[len] = 0x00;
	        len--;
	    }
	}
	void callbackPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
	{
	    callback(vm,addrSp,offset,Byt,len);
	}

	void callbackPage::cleanupAndUnlink()
	{
	    return;
	}

	void callbackPage::setParent(addressSpace* addrSp)
	{
	    addressSpace::setParent(addrSp);
	}

	callbackPage::callbackPage(addressSpace* parent,pageCallback callbackfunc)
	{

	}

	callbackPage::callbackPage(addressSpace* parent,pageCallback callbackfunc,virtualMemorySpace* vmsp,addressSpace* adrSp,unsigned int sz)
	{
	    memoryTypeID = "CallPage";
	    callback = callbackfunc;
	    vm = vmsp;
	    addrSp = adrSp;
	    pagesize = sz;
	    initialLink(parent,sz);
	}

	//callbackPage::callbackPage(unsigned int sz,pageCallback callbackfunc);
	callbackPage::~callbackPage()
	{

	}

