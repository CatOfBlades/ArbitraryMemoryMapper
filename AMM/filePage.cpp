
#include "filePage.h"

unsigned long int filePage::_size()
{
	if(fp != 0)
	{
		return lSize;
	}
	else
	{
		return 0;
	}
}

bool filePage::_is_free()
{
	if(fp == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char* filePage::_content()
{
	return NULL;
}

int filePage::RW_file(bool write, unsigned long int offs, unsigned char* dat,unsigned long int len)
{
	fseek(fp, 0, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	//unsigned long int L = len;

	if(len<=0){return;}

	if( (offs+len) > lSize ){return;}

	fseek(fp, offs, SEEK_SET);
	if(write)
	{
		fwrite((const void*)dat, 1, len, fp);
	}
	else
	{
		fread((void*)dat, 1, len, fp);
	}

}

void filePage::FMopenFile()
{
	fp = 0;
	fp = fopen(FMfilename.c_str(),"rb+"); //open file for reading and writing in binary mode.
}
void filePage::FMcloseFile()
{
	cl = fclose( fp );
	fp = 0;
}

unsigned char filePage::readByte(unsigned long int offset)
{
	unsigned char dat;
	RW_file(0, offset, &dat, 1);
	return dat;
}
void filePage::writeByte(unsigned long int offset,unsigned char Byt)
{
	RW_file(1, offset, &Byt, 1);
	return;
}
void filePage::readMem(unsigned long int offset,unsigned char* buffer, unsigned long int len)
{
	RW_file(0, offset, buffer, len);
	return;
}
void filePage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
	RW_file(1, offset, Byt, len);
	return;
}

void filePage::cleanupAndUnlink()
{
	return;
}

void filePage::setParent(addressSpace* addrSp)
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
}

filePage::filePage()
{
    fp = STDOUT_FILENO; //maybe should be stdin?? Assuming most people who want to construct a blank page are using it for interprocess communication...
	cl = 0;
	FMfilename = "";
	lSize = 0;
}

filePage::filePage(addressSpace* parent,  unsigned int Size) // so we have to make a file of a specific size...
{

}

