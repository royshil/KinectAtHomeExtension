/*
 *  Logging.cpp
 *  FireBreath
 *
 *  Created by Roy Shilkrot on 1/30/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#include "Logging.h"

boost::shared_ptr<kinectathomeAPI> plugin_jsapi_ptr;
void send_event(const std::string& etype, const std::string& edata) {
//	if(plugin_jsapi_ptr) 
//		plugin_jsapi_ptr->Event(etype,edata);
		//TODO: TBD
}
void send_log(const std::string& s) {
	if (plugin_jsapi_ptr)
		plugin_jsapi_ptr->Log(s);
}
void init_logging(const boost::shared_ptr<kinectathomeAPI>& _plugin_jsapi_ptr) {
	plugin_jsapi_ptr = _plugin_jsapi_ptr;
}