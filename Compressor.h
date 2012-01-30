/*
 *  Compressor.h
 *  FireBreath
 *
 *  Created by Roy Shilkrot on 1/26/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#define UINT64_C(val) val##ui64

#ifdef __cplusplus 
extern "C" { 
#include <libswscale/swscale.h>
#include <x264.h>
} 
#endif 

#include <XnCppWrapper.h>

class Compressor {
public:
	Compressor(xn::Context& context, xn::DepthGenerator& depthGenerator, xn::ImageGenerator& imageGenerator);
	
	void Update(const xn::DepthGenerator& depthGenerator, const xn::ImageGenerator& imageGenerator);
private:
	xn::Context& m_context;
	xn::DepthGenerator& m_depthGenerator;
	xn::ImageGenerator& m_imageGenerator;	
	
	struct SwsContext* convertCtx;
	x264_picture_t pic_in, pic_out;
	x264_t* encoder;
	int width;
	int height;
};