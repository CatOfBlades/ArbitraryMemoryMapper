
#include "multiPage.h"
#include "../Defines.h"

#ifdef WINBUILD
#include <windows.h>
#endif // WINBUILD

unsigned long int multiPage::_size()
{
    if(!bankList.size()){return 0;}
    return bankList[_bankNum]->_size();
}
bool multiPage::_is_free()
{
    if(!bankList.size()){return 0;}
    return !bankList[_bankNum]->reserved;
}
unsigned char* multiPage::_content()
{
    if(!bankList.size()){return 0;}
    return bankList[_bankNum]->_content();
}
unsigned char multiPage::readByte(unsigned long int offset)
{
    if(!bankList.size()){return 0;}
    return bankList[_bankNum]->readByte(offset);
}
void multiPage::writeByte(unsigned long int offset,unsigned char Byt)
{
    if(!bankList.size()){return;}
    bankList[_bankNum]->writeByte(offset, Byt);
    return;
}

void multiPage::readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len)
{
    if(!bankList.size()){return;}
    bankList[_bankNum]->readMem(offset, buffer, len);
    return;
}
void multiPage::writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len)
{
    if(!bankList.size()){return;}
    bankList[_bankNum]->writeMem(offset, Byt, len);
    return;
}

void multiPage::cleanupAndUnlink()
{
    while(bankList.size()>0)
    {
        //bankList.back()->cleanupAndUnlink();
            //If we handle the cleanup elsewhere it allows us even more self reference nonsense.
        bankList.pop_back();
    }
    return;
}
void multiPage::addBank(std::shared_ptr<addressSpace> AS)
{
    bankList.push_back(AS);
}
void multiPage::switchBank(int bankNum)
{
    bankNum = bankNum%bankList.size();
    _bankNum = bankNum;
}

void multiPage::removeBank(int bankNum)
{
    bankList.erase(bankList.begin()+bankNum);
}
void multiPage::removeAndUnlinkBank(int bankNum)
{
    std::shared_ptr<addressSpace> bank = bankList[bankNum];
    bankList.erase(bankList.begin()+bankNum);
}

multiPage::multiPage():addressSpace()
{
    memoryTypeID = "multpage";
    _bankNum=0;
}

