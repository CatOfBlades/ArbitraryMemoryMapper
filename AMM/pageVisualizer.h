#ifndef PAGEVISUALIZER_H_INCLUDED
#define PAGEVISUALIZER_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
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

class VisualizerWindowManager
{
public:
	addressSpace* visualizedPage;
	std::vector<quadcord> pointlist;
	LRESULT CALLBACK  static WindowProc(HWND, UINT, WPARAM, LPARAM);
	void EnableOpenGL( HDC*, HGLRC*);
	void DisableOpenGL();
	void InitPointlist();

    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    HANDLE childWindow;
    /*
	{
		return EXIT_SUCCESS;
	}
	*/

	VisualizerWindowManager(addressSpace* visualizedPage);
	~VisualizerWindowManager();

};


std::unique_ptr<VisualizerWindowManager> visualizePage(addressSpace* page);


#endif // PAGEVISUALIZER_H_INCLUDED
