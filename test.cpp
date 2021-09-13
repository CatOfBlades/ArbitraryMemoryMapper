
#include "LuaMemoryMapperLib.h"
#include <stdio.h>
using namespace std;

int main()
{
    Beep(400,100);
    //fprintf(0,"helloWorld\n");
    virtualMemorySpace vms(0);
    vms.addPage(32);
    vms.addPage(32);
    //printf("%i\n",vms.pageCount());
BYTE buf[32] = {0xde,0xad,0xbe,0xef,0x05,0xde,0xad,0xbe,0xef,0x04,0xde,0xad,0xbe,0xef,0x03,0xde,0xad,0xbe,0xef,0x02,0xde,0xad,0xbe,0xef,0x01,0xde,0xad,0xbe,0xef,0x00,0x00,0x00,};
            //0xdeadbeef05deadbeef04deadbeef03deadbeef02deadbeef01deadbeef00;
    vms.writeMem(0,(unsigned char*)buf,32);
    vms.readMem(0,(unsigned char*)buf,32);
    return 0;
}