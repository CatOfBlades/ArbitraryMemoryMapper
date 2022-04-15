
#include "LoggedPage.h"
#include "time.h"

unsigned long int loggedPage::_size()
{
    long unsigned int linkedpagesize = LinkedPage->_size();
    time_t rawtime = time(0);
    fprintf(logfile,"function _size() called got value %i, at%s\n",(int)linkedpagesize,ctime(&rawtime));
    return linkedpagesize;
}
bool loggedPage::_is_free()
{
    bool pagefree = !LinkedPage->reserved;
    time_t rawtime = time(0);
    fprintf(logfile,"function _is_free() called got value %i, at%s\n",(int)pagefree,ctime(&rawtime));
    return pagefree;
}
unsigned char* loggedPage::_content()
{
    unsigned char* rawByteAddress = LinkedPage->_content();
    time_t rawtime = time(0);
    fprintf(logfile,"function _content() called at%s\n",ctime(&rawtime));
    fprintf(logfile,"warning cannot track reads/writes to direct memory pointers such as might be returned by the _content() function.");
    fprintf(logfile,"returned pointer was:%x\n\tIf this value is zero no direct pointer was available.",rawByteAddress);
    return rawByteAddress;
}

unsigned char loggedPage::readByte(unsigned long int offset)
{
    unsigned char pagebyte = LinkedPage->readByte(offset);
    time_t rawtime = time(0);
    fprintf(logfile,"function readByte() called got value 0x%X, at%s\n",pagebyte,ctime(&rawtime));
    return pagebyte;
}
void loggedPage::writeByte(unsigned long int offset,unsigned char Byt)
{
    time_t rawtime = time(0);
    fprintf(logfile,"function writeByte(offset:%i,byte:0x%X) called, at%s\n",offset,Byt,ctime(&rawtime));
    LinkedPage->writeByte(offset,Byt);
}

void loggedPage::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
    LinkedPage->readMem(offset,buffer,len);
    time_t rawtime = time(0);
    fprintf(logfile,"function readMem(offset:%i,bufferAddress:0x%X,length:%i) called, at%s\n",offset,buffer,len,ctime(&rawtime));
}
void loggedPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    LinkedPage->writeMem(offset,Byt,len);
    time_t rawtime = time(0);
    fprintf(logfile,"function writeMem(offset:%i,bufferAddress:0x%X,length:%i) called, at%s\n",offset,Byt,len,ctime(&rawtime));
}

loggedPage::loggedPage(std::string logName,std::shared_ptr<addressSpace> page)
{
    logfile = fopen(logName.c_str(),"a");
    LinkedPage = page;
    memoryTypeID = "LogdPage";

    time_t rawtime = time(0);
    fprintf(logfile,"Log start at:%s",ctime(&rawtime));
    fprintf(logfile,"constructor: loggedPage(logfile:%s) called.\n",logName.c_str());
}
loggedPage::~loggedPage()
{
    time_t rawtime = time(0);
    fprintf(logfile,"Log end at:%s",ctime(&rawtime));
    fclose(logfile);
}
