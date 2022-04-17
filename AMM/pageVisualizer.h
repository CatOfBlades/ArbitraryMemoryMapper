#ifndef PAGEVISUALIZER_H_INCLUDED
#define PAGEVISUALIZER_H_INCLUDED

#ifdef INBUILD
#include <windows.h>
#endif
#include <gl/gl.h>
//#include <list>
#include <vector>
#include <string>
#include <memory>
#include "math/hsv2rgb.h"
#include "addressSpace.h"

struct quadcord
{
    uint8_t w;
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

DWORD WINAPI PageDisplayWindowWorkerThread(LPVOID lpParameter);

class pageVisualizer
{
    public:

    int windowClosed;

	std::shared_ptr<addressSpace> visualizedPage;
	std::vector<quadcord> pointlist;
	LRESULT CALLBACK  static WindowProc(HWND, UINT, WPARAM, LPARAM);
	void EnableOpenGL( HDC*, HGLRC*);
	void DisableOpenGL();
	void InitPointlist();

	void Visualize();

    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    HANDLE childWindow;

    pageVisualizer(std::shared_ptr<addressSpace> as);
    ~pageVisualizer();
};

class VisualizerWindowManager
{

    void cleanList();
public:

    std::vector< std::unique_ptr<pageVisualizer> > visList;

    void addVisualizer(std::shared_ptr<addressSpace> as);

	VisualizerWindowManager();
	~VisualizerWindowManager();

};


//std::unique_ptr<VisualizerWindowManager> visualizePage(void* page);


#endif // PAGEVISUALIZER_H_INCLUDED
