
#ifndef __PRIVATE_XSPNG_H
#define __PRIVATE_XSPNG_H

#include <xspng.h> //typedefs
#include <stdio.h> //FILE*

#ifdef __cplusplus
namespace xspng {
	extern "C" {
#endif

		typedef struct {
			xspng_int length;
			xspng_byte sig[4];
			xspng_byte *buffer;
			xspng_int crc;
		} xspng_chunk;

		typedef xspng_chunk* xspng_chunkp;

		void xspng_write_file_sig(FILE* fout);
		void xspng_write_int(FILE* fout, xspng_int i);
		void xspng_write_byte(FILE* fout, xspng_byte b);
		void xspng_write_chunk(FILE* fout, xspng_chunkp chunk);

		void xspng_calc_crc(xspng_chunkp chunk);
		void xspng_chunk_deflate(xspng_chunkp chunk);
		void xspng_chunk_allocate(xspng_chunkp chunk);
		void xspng_chunk_set_sig(xspng_chunkp chunk, xspng_byte *sig);
		void xspng_chunk_put_int(xspng_chunkp chunk, int i, xspng_int v);
		void xspng_chunk_put_byte(xspng_chunkp chunk, int i, xspng_byte b);

		xspng_chunkp xspng_make_IHDR(xspng_imagep img);
		xspng_chunkp xspng_make_IDAT(xspng_imagep img);
		xspng_chunkp xspng_make_IEND();

		void xspng_free_chunk(xspng_chunkp chunk);

#ifdef __cplusplus
	} //extern "C"
} //namespace xspng

#endif


#endif

