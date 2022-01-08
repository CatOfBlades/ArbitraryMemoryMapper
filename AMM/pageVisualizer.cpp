
#include "../config.h"
#ifdef BUILT_IN_VISUALIZER

#include "pageVisualizer.h"

VisualizerWindowManager::VisualizerWindowManager(addressSpace* VP)
{
    visualizedPage = VP;
    childWindow = CreateThread(0,0,PageDisplayWindowWorkerThread,this,0,0);
}
VisualizerWindowManager::~VisualizerWindowManager()
{
    Beep(600,100);
}

void VisualizerWindowManager::InitPointlist()
{

    pointlist.clear();

    if(!visualizedPage){return;}

    uint32_t pageEnd = visualizedPage->_size();
    uint32_t i = 0;
    while(i <= pageEnd)
    {
        quadcord point;
        //if random bug try forcing concurrency by using nested if statements.
        point.w = visualizedPage->readByte(i++);
        point.x = visualizedPage->readByte(i++);
        point.y = visualizedPage->readByte(i++);
        point.z = visualizedPage->readByte(i++);
        pointlist.push_back(point);
    }
}

LRESULT CALLBACK VisualizerWindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                //    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void VisualizerWindowManager::DisableOpenGL()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

DWORD WINAPI PageDisplayWindowWorkerThread(LPVOID lpParameter)
{
    VisualizerWindowManager* VWM = (VisualizerWindowManager*)lpParameter;
    WNDCLASSEX wcex;
    HWND* hwnd = &VWM->hwnd;
    HDC* hDC = &VWM->hDC;
    HGLRC* hRC = &VWM->hRC;
    std::vector<quadcord>* pointlist = &VWM->pointlist;
    MSG msg;
    BOOL bQuit = false;
    float theta = 0;
    HINSTANCE hInstance = GetModuleHandleA(0);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = VisualizerWindowManager::WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = VWM->visualizedPage->memoryTypeID;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)){return 0;}

    *hwnd = CreateWindowEx(0,
                          VWM->visualizedPage->memoryTypeID,
                          (std::string(VWM->visualizedPage->memoryTypeID)+"0").c_str(),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          256,
                          256,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    ShowWindow(*hwnd, SW_NORMAL);

    VWM->EnableOpenGL( hDC, hRC);
    VWM->InitPointlist();
    while (!bQuit)
    {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            VWM->InitPointlist();
            /* OpenGL animation code goes here */


            glClearColor(0.125f, 0.125f, 0.125f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
            glRotatef(theta, 0.0f, 0.005f, 0.005f);
            glTranslatef(-0.55f,-0.55f,-0.55f);
            glScalef(1.2f,1.2f,1.2f);


                //printf("GL pointlist loop.\n");
            glBegin(GL_LINES);
                glColor3f( 1.0f , 0.0f, 0.0f ); glVertex3f( 0.0f , 0.0f, 0.0f); glVertex3f( 1.0f , 0.0f, 0.0f);
                glColor3f( 0.0f , 1.0f, 0.0f ); glVertex3f( 0.0f , 0.0f, 0.0f); glVertex3f( 0.0f , 1.0f, 0.0f);
                glColor3f( 0.0f , 0.0f, 1.0f ); glVertex3f( 0.0f , 0.0f, 0.0f); glVertex3f( 0.0f , 0.0f, 1.0f);
                //glColor3f( 0.5f , 0.0f, 0.5f ); glVertex3f( 0.0f , 0.0f, 0.0f); glVertex4f( 0.0f , 0.0f, 0.0f, 1.0f);
            glEnd();
            glTranslatef(0.05f,0.05f,0.05f);
            glBegin(GL_POINTS);
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
                        //glColor3f( (p.w/255.0f)*((p.z)/255.0f) , (p.x/255.0f)*((p.z)/255.0f), (p.y/255.0f)*((p.z)/255.0f) );
                        glColor3f(rgb.R/255.0f,rgb.G/255.0f,rgb.B/255.0f);
                        glVertex3f( (f_rz+p.w)/255.0f, (f_rx+p.x)/255.0f , (f_ry+p.y)/255.0f );
                        //glColor3f( (p.z)/255.0f , (p.x)/255.0f, (p.y)/255.0f );
                        //glVertex3f( (p.z)/255.0f, (p.x)/255.0f , (p.y)/255.0f );
                        //i+=1;
                    }
                }

                //glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
                //glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
                //glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

            glEnd();

            glPopMatrix();

            SwapBuffers(*hDC);

            theta += 0.125f;
            //lastAutoScrollOffset = autoScrollOffset;
            //autoScrollOffset = theta;

            Sleep(1);
        }
    }
    return EXIT_SUCCESS;
}

void VisualizerWindowManager::EnableOpenGL(HDC* hdc, HGLRC* hglrc)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hdc = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hdc, &pfd);

    SetPixelFormat(*hdc, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hglrc = wglCreateContext(*hdc);

    wglMakeCurrent(*hdc, *hglrc);
}

std::unique_ptr<VisualizerWindowManager> visualizePage(addressSpace* page)
{
    return std::unique_ptr<VisualizerWindowManager>( new VisualizerWindowManager(page));
}

#endif // BUILT_IN_VISUALIZER
