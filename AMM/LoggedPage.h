
#ifndef LOGGEDPAGE_H_INCLUDED
#define LOGGEDPAGE_H_INCLUDED

#include "addressSpace.h"
#include <string>
#include <stdio.h>
#include <memory>

/** This memory page sits between one memory space and a page and logs all accesses to that page. **/

class loggedPage : public addressSpace
{
    public:
    FILE* logfile;
    std::string logfileName;
    std::shared_ptr<addressSpace> LinkedPage;
	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();

	unsigned char readByte(unsigned long int offset);
    void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();

	void setParent(addressSpace* addrSp);
	loggedPage(std::string logName,std::shared_ptr<addressSpace> page);
    //loggedPage( std::string logName,addressSpace* page);
    ~loggedPage();
};

#endif // LOGGEDPAGE_H_INCLUDED
