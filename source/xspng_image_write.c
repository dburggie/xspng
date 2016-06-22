
#include <xspng.h>
#include <private_xspng.h>

#include <stdio.h>  //fwrite, FILE*, fopen
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>

static const char 
	*fopen_fail = "xspng_image_write(): failed to open file \'%s\'\n";
	
static const char
	*chunk_fail = "xspng_image_write(): failed to generate \'%s\' chunk\n";



void xspng_image_write(xspng_imagep img, const char * filename) {
	assert(NULL != filename);
	assert(NULL != img);
	assert(NULL != img->buffer);
	assert(0 < img->width);
	assert(0 < img->height);
	
	//open file
	FILE *fout = fopen(filename, "wb");
	if (!fout) {
		fprintf(stderr, fopen_fail, filename);
		return;
	}
	
	//translate image buffer to png file chunks
	xspng_chunkp IHDR = xspng_make_IHDR(img);
	xspng_chunkp IDAT = xspng_make_IDAT(img);
	xspng_chunkp IEND = xspng_make_IEND();
	
	//ensure we got all our chunks
	int do_write = 1;
	if (!IHDR || !IHDR->sig || !IHDR->buffer) {
		fprintf(stderr, chunk_fail, "IHDR");
		do_write = 0;
	}
	
	if (!IDAT || !IDAT->sig || !IDAT->buffer) {
		fprintf(stderr, chunk_fail, "IDAT");
		do_write = 0;
	}
	
	if (!IEND || !IEND->sig) {
		fprintf(stderr, chunk_fail, "IEND");
		do_write = 0;
	}
	
	//write file
	if (do_write) {
		xspng_write_file_sig(fout);
		xspng_write_chunk(fout,IHDR);
		xspng_write_chunk(fout,IDAT);
		xspng_write_chunk(fout,IEND);
	}
	
	//close file
	fclose(fout);
	fout = NULL;
}


