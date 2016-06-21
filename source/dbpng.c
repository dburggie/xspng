
#include <dbpng.h>
#include <dbpng_private.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <zlib.h>





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

typedef struct {
	dbpng_byte sig[4];
	dbpng_int length;
	dbpng_byte *buffer;
	dbpng_int crc;
} dbpng_chunk;


typedef dbpng_chunk* dbpng_chunkp;


static const dbpng_byte sig_ihdr[4] = { 0x49, 0x48, 0x44, 0x52 };
static const dbpng_byte sig_idat[4] = { 0x49, 0x44, 0x41, 0x54 };
static const dbpng_byte sig_iend[4] = { 0x49, 0x45, 0x4e, 0x44 };



void dbpng_write_file_sig(FILE* fout);


void dbpng_write_byte(FILE* fout, dbpng_byte b) {
	fprintf(fout, "%c", b);
}



void dbpng_write_int(FILE* fout, dbpng_int i) {
	dbpng_byte array[4], j;
	for (j = 0; j < 4; j++) {
		array[j] = 0xff & i;
		i = i << 8;
	}

	for (j = 3; j >= 0; j--) {
		dbpng_write_byte(fout, array[j]);
	}
}

void dbpng_write_chunk(FILE* fout, dbpng_chunkp chunk);

dbpng_int dbpng_calc_crc(dbpng_chunkp chunk);
void dbpng_chunk_deflate(dbpng_chunkp chunk);

dbpng_chunkp dbpng_make_ihdr(dbpng_imagep img);
dbpng_chunkp dbpng_make_idat(dbpng_imagep img);
dbpng_chunkp dbpng_make_iend();



void dbpng_free_chunk(dbpng_chunkp chunk) {
	if (chunk) {
		if (chunk->buffer) {
			free(chunk->buffer);
		}
		free(chunk);
	}
}



void dbpng_write_to_file(dbpng_imagep, const char * filename) { }
