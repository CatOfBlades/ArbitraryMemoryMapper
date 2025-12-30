#include <errno.h>
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

unsigned long int filePage::RW_file(bool write, unsigned long int offs, unsigned char* dat,unsigned long int len)
{
    /*
	fseek(fp, 0, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	*/

	//unsigned long int L = len;

	if(len<=0){return 0;}

	if( (offs+len) > lSize ){return 0;}

	fseek(fp, offs, SEEK_SET);
	if(write)
	{
		fwrite((const void*)dat, 1, len, fp);
	}
	else
	{
		fread((void*)dat, 1, len, fp);
	}
    return 0;
}

void filePage::FMopenFile()
{
	fp = 0;
	
    /*first check if the file exists...*/
	int file_exists;
    fp=fopen(FMfilename.c_str(),"r");
    if (fp==NULL) file_exists=0;
    else {file_exists=1; fclose(fp);}
    
    /*...then open it in the appropriate way*/
    if (file_exists==1)
    {
       //printf("file exists!\n");
       fp=fopen(FMfilename.c_str(),"rw+b");
    }
    else
    {
        //printf("file does not exist!\n");
        fp=fopen(FMfilename.c_str(),"w+b");
    }
	
	if (fp == NULL) {
		printf("can't open %s: %i\n", FMfilename.c_str(), errno);
		exit(1);
	}

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

filePage::filePage()
{
    fp = stdout;//STDOUT_FILENO; //maybe should be stdin?? Assuming most people who want to construct a blank page are using it for interprocess communication...
	cl = 0;
	FMfilename = "";
	lSize = 0;
}

filePage::filePage(addressSpace* parent,  unsigned int Size)
{

}

filePage::filePage(std::string filename,  unsigned int Size)
{
    fp = 0;
	cl = 0;
	FMfilename = filename;
	lSize = Size;
	memoryTypeID = "filePage";
	FMopenFile();
}

