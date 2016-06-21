#include <dbpng.h>
#include <stdio.h>
#include <stdint.h>


/*
		typedef uint8_t  dbpng_byte;
		typedef uint32_t dbpng_int;

		//primary image data structure
		typedef struct {
			dbpng_int width, height;
			dbpng_byte * buffer;
		} dbpng_image;

		//pointer to the above-defined struct
		typedef dbpng_image* dbpng_imagep;

		//struct initialization functions
		dbpng_imagep dbpng_new_image(dbpng_int width, dbpng_int height);
		void dbpng_free_image(dbpng_imagep imgp);
		void dbpng_image_init(dbpng_image *img);

		//set a pixel to the given red-green-blue samples
		void dbpng_set_rgb(
				dbpng_imagep imgp, 
				dbpng_int x, dbpng_int y, 
				dbpng_byte r, dbpng_byte g, dbpng_byte b
			);

		//set a pixel to the given red-green-blue-alpha samples
		void dbpng_set_rgba(
				dbpng_imagep imgp,
				dbpng_int x, dbpng_int y,
				dbpng_byte r, dbpng_byte g, dbpng_byte b, dbpng_byte a
			);

		//write a dbpng_image struct to file
		void dbpng_write_to_file(dbpng_imagep, const char * filename);

 */



dbpng_imagep dbpng_new_image(dbpng_int width, dbpng_int height) {

	assert(0 < width);
	assert(0 < height);

	dbpng_imagep imgp = (dbpng_imagep) malloc(sizeof (dbpng_image));
	if (!imgp) return NULL;

	imgp->width = width;
	imgp->height = height;

	size_t footprint = height * (width * (4 * (sizeof(dbpng_byte))));
	
	imgp->buffer = (dbpng_byte*) malloc(footprint);
	if (!imgp->buffer) {
		free(imgp);
		return NULL;
	}

	dbpng_int y, x, s;
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



void dbpng_free_image(dbpng_imagep imgp) {
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



void dbpng_image_init(dbpng_image *img) {
	assert(0 < img->width);
	assert(0 < img->height);

	if (img->buffer) free(img->buffer);

	size_t footprint = img->width * img->height * 4 * sizeof(dbpng_byte);
	size_t stride = img->width * 4;

	img->buffer = (dbpng_byte*) malloc(footprint);
	if (!img->buffer) {
		img->width = 0;
		img->height = 0;
		return;
	}

	dbpng_int x, y;
	for (y = 0; y < img->height; y++) {
		for (x = 0; x < img->width; x++) {
			img->buffer[y*stride + x*4 + 0] = 0x00;
			img->buffer[y*stride + x*4 + 1] = 0x00;
			img->buffer[y*stride + x*4 + 2] = 0x00;
			img->buffer[y*stride + x*4 + 3] = 0xff;
		}
	}
}



void dbpng_set_rgb(
		dbpng_imagep imgp, 
		dbpng_int x, dbpng_int y, 
		dbpng_byte r, dbpng_byte g, dbpng_byte b
) {
	assert(NULL != imgp);
	assert(NULL != imgp->buffer);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < imgp->width);
	assert(y < imgp->height);

	dbpng_int stride = 4 * imgp->width;
	imgp->buffer[y * stride + x * 4    ] = r;
	imgp->buffer[y * stride + x * 4 + 1] = g;
	imgp->buffer[y * stride + x * 4 + 2] = b;
}



//set a pixel to the given red-green-blue-alpha samples
void dbpng_set_rgba(
		dbpng_imagep imgp,
		dbpng_int x, dbpng_int y,
		dbpng_byte r, dbpng_byte g, dbpng_byte b, dbpng_byte a
) {
	assert(NULL != imgp);
	assert(NULL != imgp->buffer);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < imgp->width);
	assert(y < imgp->height);

	dbpng_int stride = 4 * imgp->width;
	imgp->buffer[y * stride + x * 4    ] = r;
	imgp->buffer[y * stride + x * 4 + 1] = g;
	imgp->buffer[y * stride + x * 4 + 2] = b;
	imgp->buffer[y * stride + x * 4 + 3] = a;
}




void dbpng_write_to_file(dbpng_imagep, const char * filename) { }
