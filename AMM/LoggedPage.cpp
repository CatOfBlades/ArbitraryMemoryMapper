
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
    if (logfile != NULL)
        fprintf(logfile, "readByte() called, offset: %lu, value: 0x%02X, time: %s\n", offset, pagebyte, ctime(&rawtime));
    return pagebyte;
}

void loggedPage::writeByte(unsigned long int offset, unsigned char Byt)
{
    time_t rawtime = time(0);
    if (logfile != NULL)
        fprintf(logfile, "writeByte() called, offset: %lu, value: 0x%02X, time: %s\n", offset, Byt, ctime(&rawtime));
    LinkedPage->writeByte(offset, Byt);
}

void loggedPage::readMem(unsigned long int offset, unsigned char* buffer, unsigned long int len)
{
    time_t rawtime = time(0);
    if (logfile != NULL)
        fprintf(logfile, "readMem() called, offset: %lu, buffer address: %p, length: %lu, time: %s\n", offset, (void*)buffer, len, ctime(&rawtime));
    LinkedPage->readMem(offset, buffer, len);
}

void loggedPage::writeMem(unsigned long int offset, unsigned char* Byt, unsigned long int len)
{
    time_t rawtime = time(0);
    if (logfile != NULL)
        fprintf(logfile, "writeMem() called, offset: %lu, buffer address: %p, length: %lu, time: %s\n", offset, (void*)Byt, len, ctime(&rawtime));
    LinkedPage->writeMem(offset, Byt, len);
}


loggedPage::loggedPage(std::string logName, std::shared_ptr<addressSpace> page)
    : LinkedPage(page)
{
    // Open the logfile
    logfile = fopen(logName.c_str(), "a");
    if (logfile == nullptr) {
        #ifdef EXTRA_DEBUG_MESSAGES
        printf("Failed to open logfile: %s\n", logName.c_str());
        #endif
    }

    memoryTypeID = "LogdPage";

    time_t rawtime = time(0);
    fprintf(logfile, "Log start at: %s", ctime(&rawtime));
    fprintf(logfile, "constructor: loggedPage(logfile: %s) called.\n", logName.c_str());
}

loggedPage::~loggedPage()
{
    time_t rawtime = time(0);
    fprintf(logfile,"Log end at:%s",ctime(&rawtime));
    fclose(logfile);
}
