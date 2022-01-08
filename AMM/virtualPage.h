#ifndef VIRTUALPAGE_H_INCLUDED
#define VIRTUALPAGE_H_INCLUDED

#include "addressSpace.h"

#include "../config.h"
#ifdef BUILT_IN_VISUALIZER
    #include <memory>
    #include "pageVisualizer.h"
#endif // BUILT_IN_VISUALIZER

class virtualPage: public addressSpace
{
    #ifdef BUILT_IN_VISUALIZER
    std::unique_ptr<VisualizerWindowManager> VisWM;
    #endif // BUILT_IN_VISUALIZER
    public:
	unsigned long int _pageSize;
	bool _pageFree;
	unsigned char* _pageContent;

	unsigned long int _size();
	bool _is_free();
	unsigned char* _content();
	unsigned char readByte(unsigned long int offset);
	void writeByte(unsigned long int offset,unsigned char Byt);

	void readMem(unsigned long int offset,unsigned char* buffer ,unsigned long int len);
	void writeMem(unsigned long int offset,unsigned char* Byt,unsigned long int len);

	void cleanupAndUnlink();
	//void setParent(addressSpace* addrSp);

	virtualPage();
	virtualPage(addressSpace* parent,  unsigned int Size);
	~virtualPage();
};

#endif // VIRTUALPAGE_H_INCLUDED
