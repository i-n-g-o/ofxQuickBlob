/*//-------------------------------------------------------------------------------
 *  ofxQBlob.h
 *
 *  a simple quick blob
 *
 *  written by: Ingo Randolf - 2017
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 //-------------------------------------------------------------------------------*/

#ifndef ofxqblob_h
#define ofxqblob_h

#include "quickblob++/quickblob.h"

class ofxQBlob
{
public:
	
	ofxQBlob();
	
	ofxQBlob(blob* b) {
		size = b->size;
		color = b->color;
		center_x = b->center_x;
		center_y = b->center_y;
		bb_x1 = b->bb_x1;
		bb_y1 = b->bb_y1;
		bb_x2 = b->bb_x2;
		bb_y2 = b->bb_y2;
	};
	
	int size;
	int color;
	
	// incremental average
	double center_x;
	double center_y;
	// bounding box
	int bb_x1, bb_y1, bb_x2, bb_y2;

};

#endif /* ofxqblob_h */
