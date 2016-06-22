
#include <xspng.h>
#include <private_xspng.h>

#include <stdio.h>  //fwrite, FILE*
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>

/* begin implementation of declarations in xspng.h */

//allocate a new imagep object and allocate it's buffer
xspng_imagep xspng_new_image(xspng_int width, xspng_int height) {

	assert(0 < width);
	assert(0 < height);

	xspng_imagep imgp = (xspng_imagep) malloc(sizeof (xspng_image));
	if (!imgp) return NULL;

	imgp->width = width;
	imgp->height = height;

	size_t footprint = height * (width * (4 * (sizeof(xspng_byte))));
	
	imgp->buffer = (xspng_byte*) malloc(footprint);
	if (!imgp->buffer) {
		free(imgp);
		return NULL;
	}

	xspng_int y, x, s;
	s = width * 4;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			imgp->buffer[y*s + x*4 + 0] = 0x00;
			imgp->buffer[y*s + x*4 + 1] = 0x00;
			imgp->buffer[y*s + x*4 + 2] = 0x00;
			imgp->buffer[y*s + x*4 + 3] = 0xff;
		}
	}

	return imgp;
}



//free an imagep object (and it's buffer) you're done with
void xspng_free_image(xspng_imagep imgp) {
	if (imgp) {
		if (imgp->buffer) {
			free(imgp->buffer);
			imgp->width = 0;
			imgp->height = 0;
			imgp->buffer = NULL;
		}
		free(imgp);
	}
}



//allocate new buffer for an existing image struct
void xspng_image_init(xspng_image *img) {
	assert(0 < img->width);
	assert(0 < img->height);

	if (img->buffer) free(img->buffer);

	size_t footprint = img->width * img->height * 4 * sizeof(xspng_byte);
	size_t stride = img->width * 4;

	img->buffer = (xspng_byte*) malloc(footprint);
	if (!img->buffer) {
		img->width = 0;
		img->height = 0;
		return;
	}

	xspng_int x, y;
	for (y = 0; y < img->height; y++) {
		for (x = 0; x < img->width; x++) {
			img->buffer[y*stride + x*4 + 0] = 0x00;
			img->buffer[y*stride + x*4 + 1] = 0x00;
			img->buffer[y*stride + x*4 + 2] = 0x00;
			img->buffer[y*stride + x*4 + 3] = 0xff;
		}
	}
}



//set rgb values of a given (x,y) coordinate, leave alpha alone
void xspng_set_rgb(
		xspng_imagep imgp, 
		xspng_int x, xspng_int y, 
		xspng_byte r, xspng_byte g, xspng_byte b
) {
	assert(NULL != imgp);
	assert(NULL != imgp->buffer);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < imgp->width);
	assert(y < imgp->height);

	xspng_int stride = 4 * imgp->width;
	imgp->buffer[y * stride + x * 4    ] = r;
	imgp->buffer[y * stride + x * 4 + 1] = g;
	imgp->buffer[y * stride + x * 4 + 2] = b;
}



//set a pixel to the given red-green-blue-alpha samples
void xspng_set_rgba(
		xspng_imagep imgp,
		xspng_int x, xspng_int y,
		xspng_byte r, xspng_byte g, xspng_byte b, xspng_byte a
) {
	assert(NULL != imgp);
	assert(NULL != imgp->buffer);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < imgp->width);
	assert(y < imgp->height);

	xspng_int stride = 4 * imgp->width;
	imgp->buffer[y * stride + x * 4    ] = r;
	imgp->buffer[y * stride + x * 4 + 1] = g;
	imgp->buffer[y * stride + x * 4 + 2] = b;
	imgp->buffer[y * stride + x * 4 + 3] = a;
}



void xspng_write_to_file(xspng_imagep, const char * filename) { }


