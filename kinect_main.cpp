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

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::ImageGenerator g_ImageGenerator;

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
	glOrtho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		
		// Draw depth map
		xn::DepthMetaData depthMD;
		g_DepthGenerator.GetMetaData(depthMD); //in case we need to know the actual image format...
		DrawDepthMap(depthMD);
		
		if(g_bRecord) {
			// TODO: Save data
		}		
	}
}

// xml to initialize OpenNI
#define SAMPLE_XML_PATH "/Sample-Tracking.xml"

void kinect_stop() {
	g_Context.StopGeneratingAll();
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
	rc = g_Context.CreateAnyProductionTree(XN_NODE_TYPE_IMAGE, NULL, g_ImageGenerator, &errors);
	CHECK_ERRORS(rc, errors, "Create Image");
	
	/************ Recorder code **************/
	RecConfiguration config;
	//set up image formats / resolution
	rc = ConfigureGenerators(config, g_Context, g_DepthGenerator, g_ImageGenerator);
	CHECK_RC(rc, "Config generators");
	
	//TODO: add the Kinect@Home recorder / streamer
	/*****************************************/
	
	
	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");
	
	kinect_initialized = true;

	return rc;
}
