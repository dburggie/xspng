
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

/* end implementation of declarations in xspng.h */





//write the png-file specific magic bytes to a file handle
void xspng_write_file_sig(FILE* fout) {
	assert(NULL != fout);
	fwrite(xspng_sig_PNG, 1, 8, fout);
}



//write a chunkp object to a file handle
void xspng_write_chunk(FILE* fout, xspng_chunkp chunk) {
	assert(NULL != fout);
	assert(NULL != chunk);
	assert(NULL != chunk->sig);
	assert(NULL != chunk->buffer || 0 == chunk->length);
	
	//reverse byte order for length and crc
	xspng_byte lenbuf[4];
	xspng_byte crcbuf[4];
	int i; for (i = 0; i < 3; i++) {
		lenbuf[3-i] = 0xff & (chunk->length >> (8 * i));
		crcbuf[3-i] = 0xff & (chunk->crc >> (8 * i));
	}
	
	//write length
	fwrite(lenbuf, 1, 4, fout);
	
	//write chunk signature and data
	fwrite(chunk->sig, 1, 4 + chunk->length, fout);
	
	//write chunk crc
	fwrite(crcbuf, 1, 4, fout);
}





/* crc utilities */

static unsigned long crc_table[256];
static int crc_table_computed = 0;

static void make_crc_table() {
	unsigned long c;
	int n, k;
	
	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}

	crc_table_computed = 1;
}

//update a running crc
static xspng_int update_crc(xspng_int crc, xspng_byte * buf, size_t len) {
	xspng_int c = crc;
	int n;

	if (!crc_table_computed) make_crc_table();

	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}

	return c;
}

/* end of CRC utilites */



//set the crc value of chunk
void xspng_calc_crc(xspng_chunkp chunk) {
	size_t len = chunk->length + 4;
	chunk->crc = update_crc(0xffffffffL, chunk->sig, len) ^ 0xffffffffL;
}






//allocate a chunk's buffer to be at least big enough for it's length
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
	if (chunk->length > 0)
		chunk->buffer = &(chunk->sig[4]);
	else
		chunk->buffer = NULL;
}



//copy 4 bytes of chunk signature into chunk
void xspng_chunk_set_sig(xspng_chunkp chunk, xspng_byte *sig) {
	assert(NULL != chunk);
	assert(NULL != sig);
	assert(NULL != chunk->sig || 0 == chunk->length);

	int i;
	for (i = 0; i < 4; i++) {
		chunk->sig[i] = sig[i];
	}
}



//write a 32 bit int into the chunk buffer (msb first) at the given index
void xspng_chunk_put_int(xspng_chunkp chunk, int i, xspng_int v) {
	assert(NULL != chunk);
	assert(NULL != chunk->sig);
	assert(NULL != chunk->buffer || 0 == chunk->length);
	assert(0 <= i);
	assert(i + 3 < chunk->length);

	int j; for (j = 0; j < 4; j++) {
		chunk->buffer[i + 3 - j] = 0xff & (v >> (8*j));
	}
}



//write a byte into the buffer at the given index
void xspng_chunk_put_byte(xspng_chunkp  chunk, int i, xspng_byte v) {
	assert(NULL != chunk);
	assert(NULL != chunk->sig);
	assert(NULL != chunk->buffer || 0 == chunk->length);
	assert(0 <= i);
	assert(i < chunk->length);

	chunk->buffer[i] = v;
}



//make an IHDR chunk from an imagep object
xspng_chunkp xspng_make_IHDR(xspng_imagep img) {
	assert(NULL != img);
	assert(NULL != img->buffer);

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

	xspng_chunk_set_sig(IHDR, xspng_sig_IHDR); //ASCII "IHDR"
	xspng_chunk_put_int(IHDR, 0, img->width);  //image width
	xspng_chunk_put_int(IHDR, 4, img->height); //image height
	xspng_chunk_put_byte(IHDR, 8, 8);          //8 bit sample depth depth
	xspng_chunk_put_byte(IHDR, 9, 6);          //rgba color type
	xspng_chunk_put_byte(IHDR, 10, 0);         //deflate compression
	xspng_chunk_put_byte(IHDR, 11, 0);         //default scanline filtering
	xspng_chunk_put_byte(IHDR, 12, 1);         //Adam7 interlacing on

	xspng_calc_crc(IHDR);

	return IHDR;
}



//use zlib to compress a chunk's buffer
//updates buffer size and length field
void xspng_chunk_deflate(xspng_chunkp chunk); //todo



//make an IDAT chunk from an imagep object
//performs Adam7 interlacing and compression
xspng_chunkp xspng_make_IDAT(xspng_imagep img);



//make an IEND chunk (they are always the same
xspng_chunkp xspng_make_IEND() {
	xspng_chunkp IEND = (xspng_chunkp) malloc(sizeof (xspng_chunk));
	if (!IEND) return NULL;
	
	IEND->sig = NULL;
	IEND->buffer = NULL;
	IEND->length = 0;
	xspng_chunk_allocate(IEND);
	
	if (!IEND->sig) {
		free(IEND);
		return NULL;
	}
	
	xspng_chunk_set_sig(IEND, xspng_sig_IEND); //ASCII "IEND"
	xspng_calc_crc(IEND);
	
	return IEND;
}



//free a chunkp object
void xspng_free_chunk(xspng_chunkp chunk) {
	if (chunk) {
		if (chunk->sig) {
			free(chunk->sig);
		}
		free(chunk);
	}
}



void xspng_write_to_file(xspng_imagep, const char * filename) { }
