/*
 *  kinect_common.h
 *  FireBreath
 *
 *  Created by Roy Shilkrot on 1/18/12.
 *
 */

#pragma once

// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

// Header for NITE
#include "XnVNite.h"

#include "Logging.h"

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		send_log(std::string(what) + " failed " + std::string(xnGetStatusString(rc))); \
		return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
		XnChar strError[1024];				\
		errors.ToString(strError, 1024);	\
		printf("%s\n", strError);			\
		send_log(strError);					\
		return (rc);						\
	}

#include "ResourceRecovery.h"



XnMapOutputMode QVGAMode = { 320, 240, 30 };
XnMapOutputMode VGAMode = { 640, 480, 30 };

// Configuration
// Taken from CyclicBuffer example
struct RecConfiguration
{
	RecConfiguration()
	{
		pDepthMode = &QVGAMode;
		pImageMode = &QVGAMode;
		bRecordDepth = TRUE;
		bRecordImage = TRUE;
		
		bMirrorIndicated = FALSE;
		bMirror = FALSE;
		
		bRegister = FALSE;
		bFrameSync = FALSE;
		bVerbose = FALSE;
		
		nDumpTime = 1;
		sprintf(strDirName, ".");
	}
	XnMapOutputMode* pDepthMode;
	XnMapOutputMode* pImageMode;
	XnBool bRecordDepth;
	XnBool bRecordImage;
	
	XnBool bMirrorIndicated;
	XnBool bMirror;
	XnBool bRegister;
	XnBool bFrameSync;
	XnBool bVerbose;
	
	XnUInt32 nDumpTime;
	XnChar strDirName[XN_FILE_MAX_PATH];
};

XnStatus ConfigureGenerators(const RecConfiguration& config, xn::Context& context, xn::DepthGenerator& depthGenerator, xn::ImageGenerator& imageGenerator)
{
	XnStatus nRetVal = XN_STATUS_OK;
	xn::EnumerationErrors errors;
	
	// Configure the depth, if needed
	if (config.bRecordDepth)
	{
		nRetVal = context.CreateAnyProductionTree(XN_NODE_TYPE_DEPTH, NULL, depthGenerator, &errors);
		CHECK_ERRORS(nRetVal, errors, "Create Depth");
		nRetVal = depthGenerator.SetMapOutputMode(*config.pDepthMode);
		CHECK_RC(nRetVal, "Set Mode");
		if (config.bMirrorIndicated && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_MIRROR))
		{
			depthGenerator.GetMirrorCap().SetMirror(config.bMirror);
		}
		
		// Set Hole Filter
		depthGenerator.SetIntProperty("HoleFilter", TRUE);
	}
	// Configure the image, if needed
	if (config.bRecordImage)
	{
		nRetVal = context.CreateAnyProductionTree(XN_NODE_TYPE_IMAGE, NULL, imageGenerator, &errors);
		CHECK_ERRORS(nRetVal, errors, "Create Image");
		nRetVal = imageGenerator.SetMapOutputMode(*config.pImageMode);
		CHECK_RC(nRetVal, "Set Mode");
		
		if (config.bMirrorIndicated && imageGenerator.IsCapabilitySupported(XN_CAPABILITY_MIRROR))
		{
			imageGenerator.GetMirrorCap().SetMirror(config.bMirror);
		}
	}
	
	// Configuration for when there are both streams
	if (config.bRecordDepth && config.bRecordImage)
	{
		// Registration
		if (config.bRegister && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
		{
			nRetVal = depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
			CHECK_RC(nRetVal, "Registration");
		}
		// Frame Sync
		if (config.bFrameSync && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_FRAME_SYNC))
		{
			if (depthGenerator.GetFrameSyncCap().CanFrameSyncWith(imageGenerator))
			{
				nRetVal = depthGenerator.GetFrameSyncCap().FrameSyncWith(imageGenerator);
				CHECK_RC(nRetVal, "Frame sync");
			}
		}
	}
	
	return XN_STATUS_OK;
}
