
#include <xspng.h>
#include <private_xspng.h>

#include <stdio.h>  //fwrite, FILE*
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>

#define CHUNK_SIZE 16384

//use zlib to compress a chunk's buffer
//updates buffer size and length field
void xspng_chunk_deflate(xspng_chunkp chunk) {
	assert(NULL != chunk);
	assert(NULL != chunk->sig);
	assert(NULL != chunk->buffer);
	assert(0 < chunk->length);
	
	//todo: check zlib.h version
	
	xspng_int read_total, read_size;
	xspng_int write_total = 0;
	
	xspng_chunkp target = xspng_chunk_new();
	assert(NULL != target);
	
	target->length = 20 + 2 * chunk->length;
	xspng_chunk_allocate(target);
	assert(NULL != target->sig);
	
	xspng_byte in[CHUNK_SIZE]; //input buffer for deflate()
	xspng_byte out[CHUNK_SIZE]; //output buffer for deflate()
	
	int ret, //stores zlib return codes
		flush; //tracks flushing state of deflate()
	
	unsigned have; //amount of data returned by deflate()
	
	z_stream strm; //passes data to and from zlib routines
	
	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL; //default memory management
	ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (ret != Z_OK) {
		xspng_chunk_clear(chunk);
		return;
	}
	
	do {
		//find size of read
		if ((chunk->length - read_total) < CHUNK_SIZE)
			read_size = chunk->length - read_total;
		else
			read_size = CHUNK_SIZE;
		
		//transfer data to buffer
		memcpy(in, &(chunk->buffer[read_total]), read_size);
		read_total += read_size;
		strm.avail_in = read_size;
		
		//check if we're done reading
		flush = index == chunk->length ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;
		
		//call deflate until input is empty
		do {
			strm.avail_out = CHUNK_SIZE;
			strm.next_out = out;
			
			//call deflate
			ret = deflate(&strm, flush);
			assert(ret != Z_STREAM_ERROR);
			
			//find how much we wrote and output it
			have = CHUNK - strm.avail_out;
			assert(target->length >= (write_total + have));
			memcpy(&(target->buffer[write_total]),out,have);
			write_total += have;
			
		} while (0 == strm.avail_out); //stop when we can't fill the output
		assert(0 == strm.avail_in); //make sure we used all the input
		
	} while (flush != Z_FINISH); //go until we've got everything
	assert(ret == Z_STREAM_END); //make sure we got everything
	
	deflateEnd(&strm);
	
	target->length = 2 + write_total + 4; //two byte header 4 byte footer
	xspng_chunk_set_sig(target, chunk->sig);
	xspng_chunk_clear(chunk);
	
	//copy target over to our chunk
	chunk->length = target->length; target->length = 0;
	chunk->sig = target->sig; target->sig = NULL;
	chunk->buffer = target->buffer; target->buffer = NULL;
}


