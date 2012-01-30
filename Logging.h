/*
 *  Logging.h
 *  FireBreath
 *
 *  Created by Roy Shilkrot on 1/30/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#include <string>
#include <boost/shared_ptr.hpp>
#include "kinectathomeAPI.h"

void send_event(const std::string& etype, const std::string& edata);
void send_log(const std::string& s);
void init_logging(const boost::shared_ptr<kinectathomeAPI>& );