/*
 *  Compressor.cpp
 *  FireBreath
 *
 *  Created by Roy Shilkrot on 1/26/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#include "Compressor.h"

Compressor::Compressor(xn::Context& context, 
					   xn::DepthGenerator& depthGenerator, 
					   xn::ImageGenerator& imageGenerator):
m_context(context),
m_depthGenerator(depthGenerator),
m_imageGenerator(imageGenerator)
{
	xn::ImageMetaData imd;
	imageGenerator.GetMetaData(imd);
	width = imd.XRes();
	height = imd.YRes();
	
	//init pix format converter 
	//TODO: release in d'tor
	convertCtx = sws_getCachedContext(convertCtx, width, height, PIX_FMT_RGB24, width, height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	
	//TODO: init x264 compressor
	x264_param_t param;
	x264_param_default_preset(&param, "veryfast", "zerolatency");
	param.i_threads = 1;
	param.i_width = width;
	param.i_height = height;
	param.i_fps_num = 10;
	param.i_fps_den = 1;
	// Intra refres:
	param.i_keyint_max = 10;
	param.b_intra_refresh = 1;
	//Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 25;
	param.rc.f_rf_constant_max = 35;
	//For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;
	x264_param_apply_profile(&param, "baseline");
	
	encoder = x264_encoder_open(&param);
	x264_picture_alloc(&pic_in, X264_CSP_I420, width, height);
}

void Compressor::Update(const xn::DepthGenerator& depthGenerator, 
						const xn::ImageGenerator& imageGenerator) {
	//TODO: get RGB image buffer from generator
	xn::ImageMetaData imd;
	imageGenerator.GetMetaData(imd);
	const XnUInt8* data = imd.Data();
	
	//TODO: convert pix-fmt 
	int srcstride = width*3; //RGB stride is just 3*width
	sws_scale(convertCtx, &data, &srcstride, 0, height, pic_in.img.plane, pic_in.img.i_stride);
	
	//TODO: send to x264 compression
	x264_nal_t* nals;
	int i_nals;
	int frame_size = x264_encoder_encode(encoder, &nals, &i_nals, &pic_in, &pic_out);
	if (frame_size >= 0)
	{
		// OK
	}
}