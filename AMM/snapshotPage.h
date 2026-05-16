#ifndef SNAPSHOTPAGE_H_INCLUDED
#define SNAPSHOTPAGE_H_INCLUDED

#include "addressSpace.h"

/***************************************************************************\
| A pagetype that copies all data from another page into an internal buffer |
| forming a "snapshot" of the memory from that page.                        |
| This pagetype includes functions to update the snapshot from the source.  |
| Otherwise this pagetype acts as a regular "virtualPage" except that       |
|  the write functions do nothing, as doing so would invalidate the snapshot|
\***************************************************************************/

/*! Developer note: Add an "Update snapshot" function in luamap */

class snapshotPage: public addressSpace
{
    public:
	unsigned long int _pageSize;
	unsigned char* _pageContent;
	
	std::shared_ptr<addressSpace> linkedPage;
	void Link(std::shared_ptr<addressSpace> AS);
	void cleanupAndUnlink();

	unsigned long int _size();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void updateSnapshot();

	snapshotPage();
	snapshotPage(std::shared_ptr<addressSpace> AS);
	~snapshotPage();
};

#endif // SNAPSHOTPAGE_H_INCLUDED