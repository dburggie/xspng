
#ifndef __PRIVATE_XSPNG_H
#define __PRIVATE_XSPNG_H

#include <xspng.h> //typedefs
#include <stdio.h> //FILE*

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		xspng_int length;
		xspng_int crc;
		xspng_byte *sig;
		xspng_byte *buffer;
	} xspng_chunk;
	//note that sig and buffer are contiguous in memory
	
	typedef xspng_chunk* xspng_chunkp;
	
	
	
	//return a new chunk object with no allocated buffer
	xspng_chunkp xspng_chunk_new();
	
	//free a chunk objects buffer and then the object itself
	void xspng_chunk_free(xspng_chunkp p);
	
	//free a chunk buffer
	void xspng_chunk_clear(xspng_chunkp chunk);
	
	//allocate (or reallocate) a chunk's buffer
	void xspng_chunk_allocate(xspng_chunkp chunk);
	
	//set a chunk's 4-byte signature
	void xspng_chunk_set_sig(xspng_chunkp chunk, const xspng_byte *sig);
	
	//transform chunk-data to zlib format
	xspng_chunkp xspng_chunk_deflate(xspng_chunkp chunk);
	
	//calculate and set a chunk's crc value
	void xspng_chunk_set_crc(xspng_chunkp chunk);
	
	//write a 4-byte int into the chunk buffer
	void xspng_chunk_put_int(xspng_chunkp chunk, int i, xspng_int v);
	
	//write a single byte into the chunk buffer
	void xspng_chunk_put_byte(xspng_chunkp chunk, int i, xspng_byte b);
	
	//create chunks from an image buffer
	xspng_chunkp xspng_make_IHDR(xspng_imagep img);
	xspng_chunkp xspng_make_IDAT(xspng_imagep img);
	xspng_chunkp xspng_make_IEND();
	
	//write chunks to file
	void xspng_write_file_sig(FILE* fout);
	void xspng_write_chunk(FILE* fout, xspng_chunkp chunk);
	
#ifdef __cplusplus
} //extern "C"
#endif


#endif

