
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
    bool pagefree = LinkedPage->_is_free();
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
void loggedPage::cleanupAndUnlink()
{
    //fprintf(logfile,"","");
    time_t rawtime = time(0);
    fprintf(logfile,"%s","function: cleanupAndUnlink() called, at%s\n",ctime(&rawtime));
    LinkedPage->cleanupAndUnlink();
    delete this;
}

void loggedPage::setParent(addressSpace* addrSp)
{
    _last = addrSp;
    if(addrSp)
    {
        _next = addrSp->_next;
        if(_next == NULL)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = addrSp->_top;
            while(_search != NULL)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = addrSp->_bottom;
        }
        addrSp->_next = _this;
        _top = addrSp->_top;
    }
    else
    {
        _next = NULL;
        _bottom = NULL;
        _top = NULL;
    }
    time_t rawtime = time(0);
    fprintf(logfile,"function:setParent(addressSpace:%X) called, at:%s\n",addrSp,ctime(&rawtime));
}
loggedPage::loggedPage(std::string logName,addressSpace* page)
{
    logfile = fopen(logName.c_str(),"a");
    LinkedPage = page;
    memoryTypeID = "LogdPage";
    _this = this;
    _next = NULL;
    _last = NULL;
    _top = NULL;
    _bottom = NULL;

    LinkedPage->_this = LinkedPage;
    LinkedPage->_next = _next;
    LinkedPage->_last = _last;
    LinkedPage->_top = _top;
    LinkedPage->_bottom = _bottom;
    time_t rawtime = time(0);
    fprintf(logfile,"Log start at:%s",ctime(&rawtime));
    fprintf(logfile,"constructor: loggedPage(logfile:%s,childPage:%X) called.\n",logName.c_str(),page);
}
loggedPage::loggedPage(addressSpace* parent, std::string logName, addressSpace* page)
{
    logfile = fopen(logName.c_str(),"a");
    LinkedPage = page;
    memoryTypeID = "LogdPage";
    _last = parent;
    _this = this;
    if(parent)
    {
        _next = parent->_next;
        if(_next == NULL)
        {
            _bottom = this;

            //pages need to be informed what the new bottom of the list is.
            addressSpace* _search = parent->_top;
            while(_search != NULL)
            {
                _search->_bottom = _bottom;
                _search = _search->_next;
            }

        }
        else
        {
            _bottom = parent->_bottom;
        }
        parent->_next = _this;
        _top = parent->_top;
    }
    else
    {
        _next = NULL;
        _bottom = NULL;
        _top = NULL;
    }
    LinkedPage->_this = LinkedPage;
    LinkedPage->_next = _next;
    LinkedPage->_last = _last;
    LinkedPage->_top = _top;
    LinkedPage->_bottom = _bottom;
    time_t rawtime = time(0);
    fprintf(logfile,"Log start at:%s",ctime(&rawtime));
    fprintf(logfile,"constructor: loggedPage(parent:%,logfile:%s,childPage:%X) called.\n",parent,logName.c_str(),page);
}
loggedPage::~loggedPage()
{
    time_t rawtime = time(0);
    fprintf(logfile,"Log end at:%s",ctime(&rawtime));
    fclose(logfile);
}
