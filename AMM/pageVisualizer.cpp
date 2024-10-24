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

#ifdef BUILT_IN_VISUALIZER

#include "pageVisualizer.h"
#include "addressSpace.h"

// Function to spawn a visualizer window for a given address space.
void VisualizerWindowManager::addVisualizer(std::shared_ptr<addressSpace> as) {
    visList.push_back(std::make_unique<pageVisualizer>(as));
    cleanList();
=======
void VisualizerWindowManager::addVisualizer(std::shared_ptr<addressSpace> as, std::string pagename)
{
    visList.push_back( std::make_unique<pageVisualizer>(as,pagename) );
    //cleanList();
>>>>>>> Stashed changes
}

// Removes visualizers for closed windows from the list.
void VisualizerWindowManager::cleanList() {
    int i = 0;
    while(visList.size()>i)
    {
        if(visList.at(i)->windowClosed)
        {
            visList.erase(visList.begin()+i);
        }
        i++;
    }
}

// Launches the thread for displaying the visualizer window.
void pageVisualizer::Visualize() {
    childWindow = CreateThread(0, 0, PageDisplayWindowWorkerThread, this, 0, 0);
}

pageVisualizer::pageVisualizer(std::shared_ptr<addressSpace> as)
    : visualizedPage(as), windowClosed(0) {
    Visualize();
}

pageVisualizer::~pageVisualizer() {
    // Optional: Add cleanup logic here if needed.
=======
pageVisualizer::pageVisualizer(std::shared_ptr<addressSpace> as, std::string name)
{
    visualizedPage = as;
    windowClosed = 0;
    pagename = name;
    Visualize();
}

pageVisualizer::~pageVisualizer()
{
    //Beep(600,100);
>>>>>>> Stashed changes
}

// Initializes the list of 3D points to be visualized.
void pageVisualizer::InitPointlist() {
    pointlist.clear();

    if (!visualizedPage) return;

    uint32_t pageEnd = visualizedPage->_size();
    for (uint32_t i = 0; i <= pageEnd; ) {
        quadcord point;
        point.w = visualizedPage->readByte(i++);
        point.x = visualizedPage->readByte(i++);
        point.y = visualizedPage->readByte(i++);
        point.z = visualizedPage->readByte(i++);
        pointlist.push_back(point);
    }
}

// Windows message handler for the visualizer window.
LRESULT CALLBACK pageVisualizer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
void pageVisualizer::DisableOpenGL() {
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
=======
DWORD WINAPI PageDisplayWindowWorkerThread(LPVOID lpParameter)
{
    pageVisualizer* VWM = (pageVisualizer*)lpParameter;
    WNDCLASSEX wcex;
    HWND* hwnd = &VWM->hwnd;
    HDC* hDC = &VWM->hDC;
    HGLRC* hRC = &VWM->hRC;
    std::string pagename = *&VWM->pagename;
    std::vector<quadcord>* pointlist = &VWM->pointlist;
    MSG msg;
    BOOL bQuit = false;
    float theta = 0;
    /*** This is very much the wrong way to do it, but using "GetModuleHandleA(0)" for hInstance was returning error "1410:class exists".
    I don't know how to fix this ***/
    HINSTANCE hInstance = (HINSTANCE)&wcex; //GetModuleHandleA(0);
    //HINSTANCE hInstance = GetModuleHandleA(0);
    wcex.cbSize = sizeof(WNDCLASSEX);
>>>>>>> Stashed changes
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = pageVisualizer::WindowProc;
    wcex.hInstance = GetModuleHandleA(0);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = VWM->visualizedPage->memoryTypeID.c_str();
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
    VWM->EnableOpenGL(&VWM->hDC, &VWM->hRC);
=======
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = VWM->visualizedPage->memoryTypeID.c_str();//"test";//VWM->pagename.c_str();
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        printf("PageName: %s could not register window class. error#: %i\n",pagename.c_str(),GetLastError());
        return 0;
    }

    *hwnd = CreateWindowEx(0,
                          VWM->visualizedPage->memoryTypeID.c_str(),
                          pagename.c_str(),//(std::string(VWM->visualizedPage->memoryTypeID)+"0").c_str(),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          256,
                          256,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    if(!hwnd)
    {
        printf("PageName: %s could not create window. error#: %i\n",pagename.c_str(),GetLastError());
    }
    if(ShowWindow(*hwnd, SW_NORMAL))
    {
        printf("PageName: %s not shown. error#: %i\n",pagename.c_str(),GetLastError());
    };

    VWM->EnableOpenGL( hDC, hRC);
>>>>>>> Stashed changes
    VWM->InitPointlist();

    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            VWM->InitPointlist();
            glClearColor(0.125f, 0.125f, 0.125f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

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
=======
                if(pointlist->size())
                {
                    //int i = 0;
                    for(quadcord p : *pointlist)
                    {
                        int rx = rand();
                        float f_rx = (rx/RAND_MAX*100)-0.01f;
                        int ry = rand();
                        float f_ry = (ry/RAND_MAX*100)-0.01f;
                        int rz = rand();
                        float f_rz = (rz/RAND_MAX*100)-0.01f;

                        hsv_t hsv;
                        hsv.H = (p.y/255.0f)*360;
                        hsv.S = ((p.x/255.0f)+(p.w/255.0f))*50;
                        hsv.V = ((p.z)/255.0f)*100;
                        rgb_t rgb;
                        HSVtoRGB(hsv,&rgb);
                        glColor3f(rgb.R/255.0f,rgb.G/255.0f,rgb.B/255.0f);
                        glVertex3f( (f_rz+p.w)/255.0f, (f_rx+p.x)/255.0f , (f_ry+p.y)/255.0f );
                        //glColor3f( (p.z)/255.0f , (p.x)/255.0f, (p.y)/255.0f );
                        //glVertex3f( (p.z)/255.0f, (p.x)/255.0f , (p.y)/255.0f );
                        //i+=1;
                    }
                }
>>>>>>> Stashed changes

                rgb_t rgb;
                HSVtoRGB(hsv, &rgb);

                glColor3f(rgb.R / 255.0f, rgb.G / 255.0f, rgb.B / 255.0f);
                glVertex3f((p.w / 255.0f), (p.x / 255.0f), (p.y / 255.0f));
            }
            glEnd();

            glPopMatrix();
            SwapBuffers(VWM->hDC);

            theta += 0.125f;
            Sleep(1);
        }
    }

    return EXIT_SUCCESS;
}

// Enables OpenGL rendering.
void pageVisualizer::EnableOpenGL(HDC* hdc, HGLRC* hglrc) {
    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR)};
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    *hdc = GetDC(hwnd);
    int iFormat = ChoosePixelFormat(*hdc, &pfd);
    SetPixelFormat(*hdc, iFormat, &pfd);

    *hglrc = wglCreateContext(*hdc);
    wglMakeCurrent(*hdc, *hglrc);
}

// Constructors/destructors for VisualizerWindowManager.
VisualizerWindowManager::VisualizerWindowManager() {}
VisualizerWindowManager::~VisualizerWindowManager() {}

#endif // BUILT_IN_VISUALIZER
