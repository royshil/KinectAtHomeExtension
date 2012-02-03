/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Point Viewer Sample                                  *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#include <iostream>
#include <string>
#include "kinect_common.h"

// local header
#include "PointDrawer.h"

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#elif defined(__linux__)
#  include <GL/gl.h>
#else
#  include <gl\gl.h>
#endif

#undef USE_GLUT


//#ifdef USE_GLUT
//	#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
//		#include <GLUT/glut.h>
//	#else
//		#include <GL/glut.h>
//	#endif
//#elif defined(USE_GLES)
//	#include "opengles.h"
//	#include "kbhit.h"
//#endif
//#include "signal_catch.h"

//#ifdef USE_GLES
//static EGLDisplay display = EGL_NO_DISPLAY;
//static EGLSurface surface = EGL_NO_SURFACE;
//static EGLContext context = EGL_NO_CONTEXT;
//#endif



// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::HandsGenerator g_HandsGenerator;
xn::GestureGenerator g_GestureGenerator;
xn::ImageGenerator g_ImageGenerator;

// NITE objects
XnVSessionManager* g_pSessionManager = NULL;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

XnBool g_bRecord = false;
void kinect_setRecord(bool is_record) { g_bRecord = is_record; }

bool kinect_initialized = false;


//CyclicBuffer* cyclicBuffer = NULL;

SessionState g_SessionState = NOT_IN_SESSION;

void CleanupExit()
{
	exit (1);
}

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
//	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}

void XN_CALLBACK_TYPE TouchingCallback(xn::HandTouchingFOVEdgeCapability& generator, XnUserID id, const XnPoint3D* pPosition, XnFloat fTime, XnDirection eDir, void* pCookie)
{
	g_pDrawer->SetTouchingFOVEdge(id);
}
/*
void XN_CALLBACK_TYPE MyGestureInProgress(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s in progress\n", strGesture);
}
void XN_CALLBACK_TYPE MyGestureReady(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s ready for next stage\n", strGesture);
}
*/

//impl. in PointDrawer.cpp, from OpenNI examples
void DrawDepthMap(const xn::DepthMetaData& dm);


// this function is called each frame
void glutDisplay (void)
{
	if(!kinect_initialized) return;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	XnMapOutputMode mode;
	g_DepthGenerator.GetMapOutputMode(mode);
//#ifdef USE_GLUT
	glOrtho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);
#if defined(USE_GLES)
	glOrthof(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);
#endif

	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		// Update NITE tree
//		g_pSessionManager->Update(&g_Context); //this is for gestures, we dont need it
		
		// Draw depth map
		xn::DepthMetaData depthMD;
		g_DepthGenerator.GetMetaData(depthMD);
		DrawDepthMap(depthMD);
		
		if(g_bRecord) {
			// TODO: Save data
		}
		
#ifdef USE_GLUT
		PrintSessionState(g_SessionState);
#endif
	}

#ifdef USE_GLUT
	glutSwapBuffers();
#endif
}

#ifdef USE_GLUT
void glutIdle (void)
{
	if (g_bQuit) {
		CleanupExit();
	}

	// Display the frame
	glutPostRedisplay();
}

void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// Exit
		CleanupExit();
	case'p':
		// Toggle pause
		g_bPause = !g_bPause;
		break;
	case 'd':
		// Toggle drawing of the depth map
		g_bDrawDepthMap = !g_bDrawDepthMap;
		g_pDrawer->SetDepthMap(g_bDrawDepthMap);
		break;
	case 'f':
		g_bPrintFrameID = !g_bPrintFrameID;
		g_pDrawer->SetFrameID(g_bPrintFrameID);
		break;
	case 's':
		// Toggle smoothing
		if (g_fSmoothing == 0)
			g_fSmoothing = 0.1;
		else 
			g_fSmoothing = 0;
		g_HandsGenerator.SetSmoothing(g_fSmoothing);
		break;
	case 'e':
		// end current session
		g_pSessionManager->EndSession();
		break;
	}
}
void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("PrimeSense Nite Point Viewer");
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
#endif

void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}


// xml to initialize OpenNI
#define SAMPLE_XML_PATH "/Sample-Tracking.xml"

void kinect_stop() {
	g_pSessionManager->EndSession();
//	g_pSessionManager->RemoveListener(g_pFlowRouter);
//	g_pSessionManager->ClearAllQueues();
//	g_pSessionManager->StopPrimaryStatic();
//	
	g_Context.StopGeneratingAll();
//	std::cout << "shutdown"<<std::endl;
//	g_Context.Shutdown();
//	std::cout<<"release"<<std::endl;
//	g_Context.Release();
//
//	std::cout<<"delete g_pFlowRouter;"<<std::endl;
//	delete g_pFlowRouter;
//	std::cout<<"delete g_pDrawer;"<<std::endl;
//	delete g_pDrawer;
	
//	std::cout<<"delete g_pSessionManager;"<<std::endl;
//	delete g_pSessionManager;
}

int kinect_main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;
	
	if(kinect_initialized) { 
		g_Context.StartGeneratingAll();
		return XN_STATUS_OK;
	}

	// Initialize OpenNI
	std::string xml_file_path = getResourcesDirectory();
	rc = g_Context.InitFromXmlFile((xml_file_path + SAMPLE_XML_PATH).c_str(), g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
//	rc = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
//	CHECK_RC(rc, "Find hands generator");
//	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
//	CHECK_RC(rc, "Find gesture generator");
	rc = g_Context.CreateAnyProductionTree(XN_NODE_TYPE_IMAGE, NULL, g_ImageGenerator, &errors);
	CHECK_ERRORS(rc, errors, "Create Image");

//	XnCallbackHandle h;
//	if (g_HandsGenerator.IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE))
//	{
//		g_HandsGenerator.GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(TouchingCallback, NULL, h);
//	}
//
//	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
//	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
//	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
//	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);
//
//	// Create NITE objects
//	g_pSessionManager = new XnVSessionManager;
//	rc = g_pSessionManager->Initialize(&g_Context, "Click,Wave", "RaiseHand,Click");
//	CHECK_RC(rc, "SessionManager::Initialize");
//	g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	/********** Hand Detection Code **********/
	g_pDrawer = new XnVPointDrawer(20, g_DepthGenerator); 
	g_pFlowRouter = new XnVFlowRouter;
	g_pFlowRouter->SetActive(g_pDrawer);

// no need to actually track hands...	
//	g_pSessionManager->AddListener(g_pFlowRouter);

	g_pDrawer->RegisterNoPoints(NULL, NoHands);
	g_pDrawer->SetDepthMap(g_bDrawDepthMap);
	/*****************************************/

	
	/************ Recorder code **************/
	RecConfiguration config;
	rc = ConfigureGenerators(config, g_Context, g_DepthGenerator, g_ImageGenerator);
	CHECK_RC(rc, "Config generators");
	
	//TODO: add the Kinect@Home recorder / streamer
	/*****************************************/
	
	
	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");
	
	kinect_initialized = true;
	// Mainloop
#ifdef USE_GLUT

	glInit(&argc, argv);
	glutMainLoop();

#elif defined(USE_GLES)
	if (!opengles_init(GL_WIN_SIZE_X, GL_WIN_SIZE_Y, &display, &surface, &context))
	{
		printf("Error initializing opengles\n");
		CleanupExit();
	}
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	while ((!_kbhit()) && (!g_bQuit))
	{
		glutDisplay();
		eglSwapBuffers(display, surface);
	}
	opengles_shutdown(display, surface, context);

	CleanupExit();
#endif

	return rc;
}
