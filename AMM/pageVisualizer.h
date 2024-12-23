#ifndef PAGEVISUALIZER_H_INCLUDED
#define PAGEVISUALIZER_H_INCLUDED

#ifdef WINBUILD
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
void cleanList();
void EnableOpenGL(HWND* hwnd, HDC* hdc, HGLRC* hglrc);
void addVisualizer(std::shared_ptr<addressSpace> as);

class pageVisualizer
{
    public:
        std::shared_ptr<addressSpace> visualizedPage;
        std::vector<quadcord> pointlist;
        bool windowClosed;
        HWND hwnd;
        HDC hDC;
        HGLRC hRC;
        pageVisualizer(std::shared_ptr<addressSpace> as);
        HANDLE childWindow;
};

#endif // PAGEVISUALIZER_H_INCLUDED
