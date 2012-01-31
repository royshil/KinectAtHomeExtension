#include "kinectathomeWin.h"

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

bool kinectathomeWin::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow* window)
{
	pluginWindow = window;
	FB::PluginWindowlessWin *wndLess = dynamic_cast<FB::PluginWindowlessWin*>(window);
    pluginWindowWin = dynamic_cast<FB::PluginWindowWin*>(window);

	boost::thread t(boost::bind(&threadedOpenGLTestPlugin::drawThreaded,  this));
    return false;
}

bool kinectathomeWin::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately

    return false;
}
