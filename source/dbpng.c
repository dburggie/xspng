
#include <dbpng.h>
#include <dbpng_private.h>

#include <stdio.h>  //fwrite, FILE*
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>

static const dbpng_byte dbpng_sig_ihdr[4] = { 0x49, 0x48, 0x44, 0x52 };
static const dbpng_byte dbpng_sig_idat[4] = { 0x49, 0x44, 0x41, 0x54 };
static const dbpng_byte dbpng_sig_iend[4] = { 0x49, 0x45, 0x4e, 0x44 };

static const dbpng_byte dbpng_sig_PNG[8] = {137, 80, 78, 71, 13, 10 26, 10}




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




void dbpng_write_file_sig(FILE* fout) {
	assert(NULL != fout);
	fwrite(dbpng_sig_PNG, 1, 8, fout);
}

void dbpng_write_chunk(FILE* fout, dbpng_chunkp chunk);

dbpng_int dbpng_calc_crc(dbpng_chunkp chunk);
void dbpng_chunk_deflate(dbpng_chunkp chunk);


void dbpng_chunk_allocate(dbpng_chunkp chunk) {
	assert(NULL != chunk);
	assert(0 <= chunk->length);

	if (chunk->sig) {
		free(chunk->sig);
		chunk->sig = NULL;
		chunk->buffer = NULL;
	}

	size_t footprint = 4 + chunk->length;
	chunk->sig = (dbpng_byte*) malloc(footprint * sizeof (dbpng_byte));
	if (!chunk->sig) {
		chunk->buffer = NULL;
		return;
	}

	memset(chunk->sig, 0, footprint);
	chunk->buffer = &(chunk->sig[4]);
}


void dbpng_chunk_set_sig(dbpng_chunkp chunk, dbpng_byte *sig);
void dbpng_chunk_put_int(dbpng_chunkp chunk, int i, dbpng_int v);
void dbpng_chunk_put_byte(dbpng_chunkp  chunk, int i, dbpng_byte v);


dbpng_chunkp dbpng_make_IHDR(dbpng_imagep img) {

	static const int iwidth = 0;
	static const int iheight = 4;
	static const int idepth = 8;
	static const int ictype = 9;
	static const int icomp = 10;
	static const int ifilter = 11;
	static const int iinterlace = 12;

	dbpng_chunkp IHDR = (dbpng_chunkp) malloc(sizeof (dbpng_chunk));
	if (!IHDR) return NULL;

	IHDR->sig = NULL;
	IHDR->buffer = NULL;
	IHDR->length = 13;
	dbpng_chunk_allocate(IHDR);

	if (!IHDR->sig) {
		free(IHDR);
		return NULL;
	}

	dbpng_chunk_set_sig(IHDR, dbpng_sig_IHDR);
	dbpng_chunk_put_int(IHDR, iwidth, img->width);
	dbpng_chunk_put_int(IHDR, iheight, img->height);
	dbpng_chunk_put_byte(IHDR, idepth, 8);
	dbpng_chunk_put_byte(IHDR, ictype, 6);
	dbpng_chunk_put_byte(IHDR, icomp, 0);
	dbpng_chunk_put_byte(IHDR, ifilter, 0);
	dbpng_chunk_put_byte(IHDR, iinterlace, 1);

	dbpng_calc_crc(IHDR);

	return IHDR;
}



dbpng_chunkp dbpng_make_IDAT(dbpng_imagep img);
dbpng_chunkp dbpng_make_IEND();



void dbpng_free_chunk(dbpng_chunkp chunk) {
	if (chunk) {
		if (chunk->buffer) {
			free(chunk->buffer);
		}
		free(chunk);
	}
}



void dbpng_write_to_file(dbpng_imagep, const char * filename) { }
