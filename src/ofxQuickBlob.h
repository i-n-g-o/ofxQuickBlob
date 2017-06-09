/*//-------------------------------------------------------------------------------
 *  ofxQuickBlob.h
 *
 *  Header file for ofxQuickBlob
 *
 *  written by: Ingo Randolf - 2017
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 //-------------------------------------------------------------------------------*/

#ifndef ofxQuickBlob_h
#define ofxQuickBlob_h

#include "ofMain.h"
#include "quickblob++/quickblob.h"
#include "ofxQBlob.h"


//! \class ofxQuickBlob
class ofxQuickBlob : public quickblob
{
public:
	ofxQuickBlob(int threshold = 127, int blobMinSize = 100, int blobMaxSize = -1);
	~ofxQuickBlob();
	
	void setup(const ofPixels& pix, bool destructive = false) {
		pixels = pix;
		
		quickblob::init(destructive);
	};
	
	void extract();
	void extract(const ofPixels& pix);
	
	// get blobs
	vector<ofxQBlob> getBlobs() { return blobs; };
	
	//
	int threshold; // use -1 to disable threshold
	int blobMinSize; // use -1 to disable minSize
	int blobMaxSize; // use -1 to disable maxSize
	int blobMatchColor;
	int channel; // select which channel to use
	
	ofPixels pixels;
	
private:
	void log_blob_hook(struct blob* b);
	int init_pixel_stream_hook(struct stream_state* stream);
	int close_pixel_stream_hook(struct stream_state* stream);
	int next_row_hook(struct stream_state* stream);
	int next_frame_hook(struct stream_state* stream);
	
	// list of blobs
	vector<ofxQBlob> blobs;
};

#endif /* ofxQuickBlob_h */
