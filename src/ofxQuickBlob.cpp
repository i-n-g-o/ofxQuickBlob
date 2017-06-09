/*//-------------------------------------------------------------------------------
 *  ofxQuickBlob.cpp
 *
 *  Implementation file for ofxQuickBlob
 *
 *  written by: Ingo Randolf - 2017
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 //-------------------------------------------------------------------------------*/

#include "ofxQuickBlob.h"
#include "ofxQBlob.h"


ofxQuickBlob::ofxQuickBlob(int thresh, int minSize, int maxSize) :
	threshold(thresh),
	blobMinSize(minSize),
	blobMaxSize(maxSize),
	blobMatchColor(-1),
	channel(0)
{
}

ofxQuickBlob::~ofxQuickBlob()
{
}


void ofxQuickBlob::log_blob_hook(struct blob* b)
{
	if (b->size < blobMinSize) {
		return;
	}
	
	if (blobMaxSize >= 0 && b->size > blobMaxSize) {
		return;
	}
	
	if (blobMatchColor >= 0 && b->color != blobMatchColor) {
		return;
	}
	

	// collect blob
	ofxQBlob newblob(b);
	blobs.push_back(newblob);
}

/*
 called on init...
 we need to have width and size here!
 */
int ofxQuickBlob::init_pixel_stream_hook(struct stream_state* stream)
{
	stream->w = (int)pixels.getWidth();
	stream->h = (int)pixels.getHeight() - 1;
	
	return 0;
}

int ofxQuickBlob::close_pixel_stream_hook(struct stream_state* stream)
{
	// nop
	return 0;
}

int ofxQuickBlob::next_row_hook(struct stream_state* stream)
{
	// get next row of image...
	
	// y: stream->y
	unsigned char* data = (unsigned char*)(pixels.getData() + (pixels.getBytesStride() * stream->y));
	
	if (isDestructive()) {
		// use the pixels
		stream->row = data;
	} else {
		// copy the pixels
		memcpy(stream->row, (void*)data, stream->w * sizeof(unsigned char));
	}
	
	if (threshold < 0) {
		return 0;
	}
	
	for (int x=0; x < stream->w; x++) {
		stream->row[x] = (stream->row[x] >= threshold) * 255;
	}
	
	return 0;
}

int ofxQuickBlob::next_frame_hook(struct stream_state* stream)
{	
	// nop - always true
	return 0;
}

void ofxQuickBlob::extract()
{
	blobs.clear();
	
	extract_image();
}

void ofxQuickBlob::extract(const ofPixels& pix)
{
	// check pixels
	int channels = pix.getNumChannels();
	int bpp = pix.getBitsPerPixel() / channels;
	
	// only one channel 8-bit
	if (bpp != 8) {
		ofLogError() << "only one channel 8-bit data - skipping blob detection";
		return;
	}
	
	// check for different sizes
	if (pixels.getWidth() > 0 && pixels.getWidth() != pix.getWidth() ) {
		// error in width
		ofLogError() << "pixels to analyze of different width! " << pixels.getWidth() << " != " << pix.getWidth();
		return;
	}
	
	if (pixels.getHeight() > 0 && pixels.getHeight() != pix.getHeight() ) {
		// error in height
		ofLogError() << "pixels to analyze of different height! " << pixels.getHeight() << " != " << pix.getHeight();
		return;
	}
	
	if (channels == 1) {
		// set the pixels
		pixels = pix;
	} else {
		
		if (channel >= channels) {
			ofLogError() << "can not use channel " << channel << " on ofPixels with: " << channels << " channels";
			return;
		}
		
		ofLogVerbose() << "using channel " << channel;
		pixels = (pix.getChannel(channel));
	}

	extract();
}
