
#ifndef MULTIPAGE_H_INCLUDED
#define MULTIPAGE_H_INCLUDED

#include "addressSpace.h"
#include "virtualMemorySpace.h"
#include <vector>
using namespace std;

/**
    This interface is built to facilitate "bank swapping" of pages.
    With this you can specify multiple pages and dynamically swap them.
**/

class multiPage: public addressSpace
{
    public:

    vector<addressSpace*> bankList;
	int _bankNum;

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	void addBank(addressSpace* AS);
	void switchBank(int bankNum);
	void removeBank(int bankNum); //Use this if the page at 'bankNum' is referenced elsewhere.
	void removeAndUnlinkBank(int bankNum); //dangerous, only do this if it's not referenced elsewhere.
    multiPage();
};


#endif // MULTIPAGE_H_INCLUDED
