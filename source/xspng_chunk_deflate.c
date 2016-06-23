
#include <xspng.h>
#include <private_xspng.h>

#include <stdio.h>  //fwrite, FILE*
#include <stdint.h> //uint8_t, uint32_t
#include <stdlib.h> //malloc, free
#include <string.h> //memset
#include <assert.h> //assert
#include <zlib.h>


#define CHUNK_SIZE 16384

//steps to deflate a stream:
//make a big buffer to store to (note it's size)
//initialize a z_stream struct
//call deflateInit(), and check it's return value
//set the source and target buffers of the z_stream
//set the avail_in and avail_out values
//call deflate() and check it's return value
//make sure we did the whole read
//call deflateEnd()
//transfer buffer to the chunk we're keeping



//use zlib to compress a chunk's buffer
//updates buffer size and length field
xspng_chunkp xspng_chunk_deflate(xspng_chunkp chunk) {
	assert(NULL != chunk);
	assert(NULL != chunk->sig);
	assert(NULL != chunk->buffer);
	assert(0 < chunk->length);
	
	//todo: check zlib.h version
	
	xspng_int buffer_size = 256 + 5 * chunk->length / 4;
	
	//set up deflated buffer
	xspng_chunkp target = xspng_chunk_new();
	assert(NULL != target);
	target->length = buffer_size - 4;
	xspng_chunk_allocate(target)
	assert(NULL != target->sig);
	xspng_chunk_set_sig(target, chunk->sig);
	
	//set up vars for use by zlib
	int ret;
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	
	ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (ret != Z_OK) {
		xspng_chunk_clear(chunk);
		xspng_chunk_free(target);
		return;
	}
	
	//prepare for deflate
	strm.avail_in = chunk->length;
	strm.next_in = chunk->buffer;
	strm.avail_out = target->length;
	strm.next_out = target->buffer;
	
	//perform deflate
	ret = deflate(&strm, Z_FINISH);
	
	//make sure we're all done
	assert(ret != Z_STREAM_ERROR);
	assert(0 < strm.avail_out);
	assert(0 == strm.avail_in);
	target->length = target->length - strm.avail_out;
	assert(0 <= target->length && target->length <= buffer_size - 4);
	assert(ret == Z_STREAM_END);
	
	//end the stream
	deflateEnd(&strm);
	
	//return the compressed chunk
	return target;
}


