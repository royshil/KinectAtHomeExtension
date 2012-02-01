#include "kinectathomeWin.h"

void glutDisplay (void);

kinectathomeWin::kinectathomeWin() { } //c'tor
kinectathomeWin::~kinectathomeWin() { } //d'tor

// OpenGL thread
void kinectathomeWin::drawThreaded()
{
	send_log("kinectathomeWin::drawThreaded");
	EnableOpenGL( pluginWindowWin->getHWND(), &hDC, &hRC );
	SetFocus(pluginWindowWin->getHWND());	
	//FB::
	FBLOG_INFO("BrowserHost", "Logging to HTML: " << "Bla!");
	static int fps = 1;
	static double start = 0, diff, wait;
	wait = 1 / fps;

	//return 0;

	while(run)
	{
		try
		{
		FB::Rect pos = pluginWindow->getWindowPosition();
		#if FB_WIN
			//HDC hDC;
			//FB::PluginWindowlessWin *wndLess = dynamic_cast<FB::PluginWindowlessWin*>(pluginWindow);
			//FB::PluginWindowWin *wnd = dynamic_cast<FB::PluginWindowWin*>(pluginWindow);
			PAINTSTRUCT ps;
			/*if (wndLess) {
				hDC = wndLess->getHDC();
			} else if (wnd) {*/
			if(pluginWindowWin){
				
				//if(!hDC)
					hDC = BeginPaint(pluginWindowWin->getHWND(), &ps);

				pos.right -= pos.left;
				pos.left = 0;
				pos.bottom -= pos.top;
				pos.top = 0;

				
				glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				glutDisplay();	
										
				SwapBuffers( hDC );

			}
			::SetTextAlign(hDC, TA_CENTER|TA_BASELINE);
			LPCTSTR pszText = _T("FireBreath Plugin!\n:-)");

			::TextOut(hDC, pos.left + (pos.right - pos.left) / 2, pos.top + (pos.bottom - pos.top) / 2, pszText, lstrlen(pszText));
			
			if (pluginWindowWin) {
				// Release the device context
				EndPaint(pluginWindowWin->getHWND(), &ps);
			}
		#endif
		//return true;
		}catch(...) 
		{
			return;
		}
		    
		
		//doesnt work
		/*
			diff = GetTickCount() - start;
			if (diff < wait) {
				Sleep(wait - diff);
			}
			start = GetTickCount();*/
		Sleep(10);

		
	}//end of while run
}


void kinectathomeWin::EnableOpenGL(HWND handleWnd, HDC * hdc, HGLRC * hRC)
{
	
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	// get the device context (DC)
	*hdc = GetDC( handleWnd );

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hdc, &pfd );
	SetPixelFormat( *hdc, format, &pfd );

	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hdc );
	wglMakeCurrent( *hdc, *hRC );

	hDC = *hdc;
}
// Disable OpenGL

void kinectathomeWin::DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

bool kinectathomeWin::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindowWin* window)
{
	send_log("kinectathomeWin::onWindowAttached");
	pluginWindow = window;
	FB::PluginWindowlessWin *wndLess = dynamic_cast<FB::PluginWindowlessWin*>(window);
    pluginWindowWin = dynamic_cast<FB::PluginWindowWin*>(window);

	boost::thread t(boost::bind(&kinectathomeWin::drawThreaded,  this));
    return kinectathome::onWindowAttached(evt,window);
}

bool kinectathomeWin::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindowWin* window)
{
	send_log("kinectathomeWin::onWindowDetached");
    // The window is about to be detached; act appropriately

    return kinectathome::onWindowDetached(evt,window);
}
