
#include <xspng.h>
#include <private_xspng.h>

#include <stdio.h>  //fwrite, FILE*
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>

static const xspng_byte xspng_sig_ihdr[4] = { 0x49, 0x48, 0x44, 0x52 };
static const xspng_byte xspng_sig_idat[4] = { 0x49, 0x44, 0x41, 0x54 };
static const xspng_byte xspng_sig_iend[4] = { 0x49, 0x45, 0x4e, 0x44 };

static const xspng_byte xspng_sig_PNG[8] = {137, 80, 78, 71, 13, 10 26, 10}




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




void xspng_write_file_sig(FILE* fout) {
	assert(NULL != fout);
	fwrite(xspng_sig_PNG, 1, 8, fout);
}

void xspng_write_chunk(FILE* fout, xspng_chunkp chunk);

xspng_int xspng_calc_crc(xspng_chunkp chunk);
void xspng_chunk_deflate(xspng_chunkp chunk);


void xspng_chunk_allocate(xspng_chunkp chunk) {
	assert(NULL != chunk);
	assert(0 <= chunk->length);

	if (chunk->sig) {
		free(chunk->sig);
		chunk->sig = NULL;
		chunk->buffer = NULL;
	}

	size_t footprint = 4 + chunk->length;
	chunk->sig = (xspng_byte*) malloc(footprint * sizeof (xspng_byte));
	if (!chunk->sig) {
		chunk->buffer = NULL;
		return;
	}

	memset(chunk->sig, 0, footprint);
	chunk->buffer = &(chunk->sig[4]);
}


void xspng_chunk_set_sig(xspng_chunkp chunk, xspng_byte *sig);
void xspng_chunk_put_int(xspng_chunkp chunk, int i, xspng_int v);
void xspng_chunk_put_byte(xspng_chunkp  chunk, int i, xspng_byte v);


xspng_chunkp xspng_make_IHDR(xspng_imagep img) {

	static const int iwidth = 0;
	static const int iheight = 4;
	static const int idepth = 8;
	static const int ictype = 9;
	static const int icomp = 10;
	static const int ifilter = 11;
	static const int iinterlace = 12;

	xspng_chunkp IHDR = (xspng_chunkp) malloc(sizeof (xspng_chunk));
	if (!IHDR) return NULL;

	IHDR->sig = NULL;
	IHDR->buffer = NULL;
	IHDR->length = 13;
	xspng_chunk_allocate(IHDR);

	if (!IHDR->sig) {
		free(IHDR);
		return NULL;
	}

	xspng_chunk_set_sig(IHDR, xspng_sig_IHDR);
	xspng_chunk_put_int(IHDR, iwidth, img->width);
	xspng_chunk_put_int(IHDR, iheight, img->height);
	xspng_chunk_put_byte(IHDR, idepth, 8);
	xspng_chunk_put_byte(IHDR, ictype, 6);
	xspng_chunk_put_byte(IHDR, icomp, 0);
	xspng_chunk_put_byte(IHDR, ifilter, 0);
	xspng_chunk_put_byte(IHDR, iinterlace, 1);

	xspng_calc_crc(IHDR);

	return IHDR;
}



xspng_chunkp xspng_make_IDAT(xspng_imagep img);
xspng_chunkp xspng_make_IEND();



void xspng_free_chunk(xspng_chunkp chunk) {
	if (chunk) {
		if (chunk->buffer) {
			free(chunk->buffer);
		}
		free(chunk);
	}
}



void xspng_write_to_file(xspng_imagep, const char * filename) { }
