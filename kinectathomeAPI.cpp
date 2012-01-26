/**********************************************************\

  Auto-generated kinectathomeAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"
#include "DOM/Window.h"

#include "kinectathomeAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn kinectathomeAPI::kinectathomeAPI(const kinectathomePtr& plugin, const FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
kinectathomeAPI::kinectathomeAPI(const kinectathomePtr& plugin, const FB::BrowserHostPtr& host) : m_plugin(plugin), m_host(host)
{
    registerMethod("echo",      make_method(this, &kinectathomeAPI::echo));
    registerMethod("testEvent", make_method(this, &kinectathomeAPI::testEvent));

    // Read-write property
    registerProperty("testString",
                     make_property(this,
                        &kinectathomeAPI::get_testString,
                        &kinectathomeAPI::set_testString));

    // Read-only property
    registerProperty("version",
                     make_property(this,
                        &kinectathomeAPI::get_version));
}

///////////////////////////////////////////////////////////////////////////////
/// @fn kinectathomeAPI::~kinectathomeAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
kinectathomeAPI::~kinectathomeAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn kinectathomePtr kinectathomeAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
kinectathomePtr kinectathomeAPI::getPlugin()
{
    kinectathomePtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}



// Read/Write property testString
std::string kinectathomeAPI::get_testString()
{
    return m_testString;
}
void kinectathomeAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string kinectathomeAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

// Method echo
FB::variant kinectathomeAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo(msg, n++);
    return msg;
}

void kinectathomeAPI::testEvent(const FB::variant& var)
{
    fire_fired(var, true, 1);
}

void kinectathomeAPI::Log(const std::string& s){
	// Retrieve a reference to the DOM Window
	FB::DOM::WindowPtr window = m_host->getDOMWindow();
	
	// Check if the DOM Window has an the property console
	if (window && window->getJSObject()->HasProperty("console")) {
		// Create a reference to the browswer console object
		FB::JSObjectPtr obj = window->getProperty<FB::JSObjectPtr>("console");
		
		// Invoke the "log" method on the console object
		obj->Invoke("log", FB::variant_list_of(s));
	}
}

