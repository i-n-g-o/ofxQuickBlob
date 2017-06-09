/*//----------------------------------------------
 * original code by Kyle Keen 2008-2017
 *
 * http://kmkeen.com/quickblob/index.html
 * https://github.com/keenerd/quickblob
 *
 * cpp version written by Ingo Randolf 2017
 * (added destructive mode)
 *
 //-----------------------------------------------*/


#ifndef quickBlob_h
#define quickBlob_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// licensed LGPL

/*
ABOUT
    image is streamed row by row
        stream_state keeps track of this
    blobs are assembled incrementally
    complete blobs are passed to log_blob_hook
    see more details in quickblob.c
*/

/* some structures you'll be working with */

struct blob
// you'll probably only need size, color, center_x and center_y
{
    int size;
    int color;
    // track current line segment
    int x1;
    int x2;
    int y;
    // basic linked list
    struct blob* prev;
    struct blob* next;
    // siblings are connected segments
    struct blob* sib_p;
    struct blob* sib_n;
    // incremental average
    double center_x;
    double center_y;
    // bounding box
    int bb_x1, bb_y1, bb_x2, bb_y2;
    // single linked list for tracking all old pixels
    // struct blob* old;
};

struct stream_state
// make a struct to hold an state required by the image loader
// and reference in the handle pointer
{
    int w, h, x, y;
    int wrap;  // don't touch this
    unsigned char* row;
    void* handle;
};

struct blob_list
{
	struct blob* head;
	int length;
	struct blob** empties;  // stack
	int empty_i;
};


class quickblob
{
	
public:
	quickblob();
	~quickblob();
	
	int init(bool destructive);
	int extract_image();
	
	bool isDestructive() { return destructive; };

	/* these are the functions you need to define
	 * you get void pointer for passing around useful data */
	
	virtual void log_blob_hook(struct blob* b) = 0;
	// the blob struct will be for a completely finished blob
	// you'll probably want to printf() important parts
	// or write back to something in user_struct
	
	virtual int init_pixel_stream_hook(struct stream_state* stream) = 0;
	// you need to set several variables:
	// stream->w = image width
	// stream->h = image hight
	// return status (0 for success)
	
	virtual int close_pixel_stream_hook(struct stream_state* stream) = 0;
	// free up anything you allocated in init_pixel_stream_hook
	// return status (0 for success)
	
	virtual int next_row_hook(struct stream_state* stream) = 0;
	// load the (grayscale) row at stream->y into the (8 bit) stream->row array
	// return status (0 for success)
	
	virtual int next_frame_hook(struct stream_state* stream) = 0;
	// basically a no-op in the library, but useful for applications
	// return status (0 for success, otherwise breaks the video loop)

	
private:
	void blank(struct blob* b);
	int init_pixel_stream(struct stream_state* stream);
	int close_pixel_stream(struct stream_state* stream);
	int malloc_blobs(struct blob_list* blist);
	int init_blobs(struct blob_list* blist);
	void blob_unlink(struct blob* b2);
	void blob_insert(struct blob* bl_start, struct blob* b2);
	int next_row(struct stream_state* stream);
	int next_frame(struct stream_state* stream);
	int scan_segment(struct stream_state* stream, struct blob* b);
	void bbox_update(struct blob* b, int x1, int x2, int y1, int y2);
	void blob_update(struct blob* b, int x1, int x2, int y);
	void sib_link(struct blob* b1, struct blob* b2);
	void blob_merge(struct blob* b1, struct blob* b2);
	int range_overlap(int a1, int a2, int b1, int b2);
	int blob_overlap(struct blob* b, int x1, int x2);
	void blob_reap(struct blob_list* blist, struct blob* b);
	void sib_cleanup(struct blob_list* blist, struct blob* b);
	void sib_find(struct blob* blob_start, struct blob* blob_now);
	void flush_incremental(struct blob_list* blist, struct blob* blob_now);
	void flush_old_blobs(struct blob_list* blist, int y);
	struct blob* empty_blob(struct blob_list* blist);

	struct stream_state stream;
	struct blob_list blist;
	struct blob* blob_now = NULL;
	struct blob* blob_prev = NULL;
	
	bool destructive;
};



#endif /* quickBlob_h */
