#include "../kinectathome.h"
#include "../kinectathomeAPI.h"

#ifdef FB_WIN
#include "PluginWindowWin.h"
#include "PluginWindowlessWin.h"
#include "PluginEvents/WindowsEvent.h"
#ifdef HAS_LEAKFINDER
#define XML_LEAK_FINDER
#include "LeakFinder/LeakFinder.h"
#endif
#endif

#ifdef HAS_LEAKFINDER
boost::scoped_ptr<LeakFinderXmlOutput> FBTestPlugin::pOut;
#endif

//openGL stuff
//opengl thread stuff
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>

HANDLE openGLThreadHandle;
DWORD dwThreadID;

float theta = 0.0f;
#include <gl\gl.h>
HWND hWnd;
HDC hDC;
HGLRC hRC;
GLfloat		rtri = 0.1f;	
bool run = true;

FB::PluginWindowWin* pluginWindowWin;
FB::PluginWindow* pluginWindow;

#include "../kinectathome.h"
#include "../Logging.h"

class kinectathomeWin : public kinectathome {
private:
	void EnableOpenGL(HWND handleWnd, HDC * hdc, HGLRC * hRC);
	void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
public:
	VOID drawThreaded();

	BEGIN_PLUGIN_EVENT_MAP()
	EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindowWin)
	EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindowWin)
	PLUGIN_EVENT_MAP_CASCADE(kinectathome)
    END_PLUGIN_EVENT_MAP()

	kinectathomeWin();
	~kinectathomeWin();

	virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindowWin *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindowWin *);

};
