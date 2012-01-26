#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for Kinect@Home Plugin
#
#\**********************************************************/

set(PLUGIN_NAME "kinectathome")
set(PLUGIN_PREFIX "KPL")
set(COMPANY_NAME "mitmedialab")

# ActiveX constants:
set(FBTYPELIB_NAME kinectathomeLib)
set(FBTYPELIB_DESC "kinectathome 1.0 Type Library")
set(IFBControl_DESC "kinectathome Control Interface")
set(FBControl_DESC "kinectathome Control Class")
set(IFBComJavascriptObject_DESC "kinectathome IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "kinectathome ComJavascriptObject Class")
set(IFBComEventSource_DESC "kinectathome IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 75c50098-61a4-5cc8-b9f6-651426a6605d)
set(IFBControl_GUID 139b8c40-9913-5e04-aef1-02ed3393d665)
set(FBControl_GUID 33192714-ad31-58bf-ba4d-f2c23bf1cc4b)
set(IFBComJavascriptObject_GUID 1c533c2e-2b04-5c86-8452-61017e1fff9e)
set(FBComJavascriptObject_GUID b2bdcb3f-0eb9-51f1-a71d-935c4bc01a33)
set(IFBComEventSource_GUID 375c25a1-6217-5bb5-a482-b20eca5f83d2)

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "mitmedialab.kinectathome")
set(MOZILLA_PLUGINID "kinectathome.media.mit.edu/kinectathome")

# strings
set(FBSTRING_CompanyName "MIT Media Lab")
set(FBSTRING_FileDescription "Native plugin for Kinect@Home project")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2012 MIT Media Lab")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "Kinect@Home Plugin")
set(FBSTRING_FileExtents "")
set(FBSTRING_PluginName "Kinect@Home Plugin")
set(FBSTRING_MIMEType "application/x-kinectathome")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
