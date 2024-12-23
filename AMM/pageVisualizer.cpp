/**************************************************
* This file defines a visualization method inspired
* by Cristopher Domes' cantor.dust program.
*
* A window is created for each page (address space)
* linked to the memory map, drawing 3D axes and a
* point for each 4 bytes in that page. W corresponds
* to a color value, and X, Y, Z form the coordinates.
*
* Currently, this module only supports Windows.
***************************************************/

#include "../config.h"
#include <thread>

#ifdef BUILT_IN_VISUALIZER

#include "pageVisualizer.h"
#include "addressSpace.h"

DWORD WINAPI PageDisplayWindowWorkerThread(LPVOID lpParameter);

// Global variables
std::vector<std::unique_ptr<pageVisualizer>> visList; // list of visualizer windows

void Visualize(pageVisualizer& visualizer) {
    visualizer.childWindow = CreateThread(0, 0, PageDisplayWindowWorkerThread, &visualizer, 0, 0);
}

// Function to spawn a visualizer window for a given address space. called only in the main thread.
void addVisualizer(std::shared_ptr<addressSpace> as) {
    visList.push_back(std::make_unique<pageVisualizer>(as));
    cleanList();
    Visualize(*visList.back());
}

pageVisualizer::pageVisualizer(std::shared_ptr<addressSpace> as)
    : visualizedPage(as), windowClosed(0) {
}

// Removes visualizers for closed windows from the list. called only in the main thread.
void cleanList() {
    int i = 0;
    while(visList.size() > i) {
        if(visList.at(i)->windowClosed) {
            visList.erase(visList.begin() + i);
        }
        i++;
    }
}

// Initializes the list of 3D points to be visualized.
void InitPointlist(pageVisualizer& visualizer) {
    visualizer.pointlist.clear();

    if (!visualizer.visualizedPage) return;

    uint32_t pageEnd = visualizer.visualizedPage->_size();
    for (uint32_t i = 0; i <= pageEnd; ) {
        quadcord point;
        point.w = visualizer.visualizedPage->readByte(i++);
        point.x = visualizer.visualizedPage->readByte(i++);
        point.y = visualizer.visualizedPage->readByte(i++);
        point.z = visualizer.visualizedPage->readByte(i++);
        visualizer.pointlist.push_back(point);
    }
}

// Windows message handler for the visualizer window.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Disables OpenGL rendering.
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

// Thread function to create and manage the OpenGL window.
DWORD WINAPI PageDisplayWindowWorkerThread(LPVOID lpParameter) {
    pageVisualizer* VWM = static_cast<pageVisualizer*>(lpParameter);
    MSG msg;
    BOOL bQuit = false;
    float theta = 0;
    HWND* hwnd = &VWM->hwnd;

    WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = GetModuleHandleA(0);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = (VWM->visualizedPage->memoryTypeID.c_str());
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) return 0;

    *hwnd = CreateWindowEx(0,
        VWM->visualizedPage->memoryTypeID.c_str(),
        (VWM->visualizedPage->memoryTypeID + "0").c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        256, 256,
        NULL, NULL,
        wcex.hInstance,
        NULL);

    ShowWindow(*hwnd, SW_NORMAL);
    EnableOpenGL(hwnd, &VWM->hDC, &VWM->hRC);
    InitPointlist(*VWM);

    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            InitPointlist(*VWM);
            glClearColor(0.125f, 0.125f, 0.125f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            glRotatef(theta, 0.0f, 0.005f, 0.005f);
            glTranslatef(-0.55f, -0.55f, -0.55f);
            glScalef(1.2f, 1.2f, 1.2f);

            glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
            glEnd();

            glBegin(GL_POINTS);
            for (const quadcord& p : VWM->pointlist) {
                hsv_t hsv;
                hsv.H = (p.y / 255.0f) * 360;
                hsv.S = ((p.x / 255.0f) + (p.w / 255.0f)) * 50;
                hsv.V = (p.z / 255.0f) * 100;

                rgb_t rgb;
                HSVtoRGB(hsv, &rgb);

                glColor3f(rgb.R / 255.0f, rgb.G / 255.0f, rgb.B / 255.0f);
                glVertex3f((p.w / 255.0f), (p.x / 255.0f), (p.y / 255.0f));
            }
            glEnd();

            glPopMatrix();
            SwapBuffers(VWM->hDC);

            theta += GL_ROTATION_SPEED;
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }

    return EXIT_SUCCESS;
}

// Enables OpenGL rendering.
void EnableOpenGL(HWND* hwnd, HDC* hdc, HGLRC* hglrc) {
    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR)};
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    *hdc = GetDC(*hwnd);
    int iFormat = ChoosePixelFormat(*hdc, &pfd);
    SetPixelFormat(*hdc, iFormat, &pfd);

    *hglrc = wglCreateContext(*hdc);
    wglMakeCurrent(*hdc, *hglrc);
}

#endif // BUILT_IN_VISUALIZER
