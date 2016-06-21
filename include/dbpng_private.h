
#ifndef __DBPNG_PRIVATE_H
#define __DBPNG_PRIVATE_H

#include <dbpng.h>
#include <stdio.h>

#ifdef __cplusplus
namespace dbpng {
	extern "C" {
#endif
		typedef struct {
			dbpng_int length;
			dbpng_byte sig[4];
			dbpng_byte *buffer;
			dbpng_int crc;
		} dbpng_chunk;

		typedef dbpng_chunk* dbpng_chunkp;

		void dbpng_write_file_sig(FILE* fout);
		void dbpng_write_int(FILE* fout, dbpng_int i);
		void dbpng_write_byte(FILE* fout, dbpng_byte b);
		void dbpng_write_chunk(FILE* fout, dbpng_chunkp chunk);

		void dbpng_calc_crc(dbpng_chunkp chunk);
		void dbpng_chunk_deflate(dbpng_chunkp chunk);
		void dbpng_chunk_allocate(dbpng_chunkp chunk);
		void dbpng_chunk_set_sig(dbpng_chunkp chunk, dbpng_byte *sig);
		void dbpng_chunk_put_int(dbpng_chunkp chunk, int i, dbpng_int v);
		void dbpng_chunk_put_byte(dbpng_chunkp chunk, int i, dbpng_byte b);

		dbpng_chunkp dbpng_make_IHDR(dbpng_imagep img);
		dbpng_chunkp dbpng_make_IDAT(dbpng_imagep img);
		dbpng_chunkp dbpng_make_IEND();

		void dbpng_free_chunk(dbpng_chunkp chunk);

#ifdef __cplusplus
	} //extern "C"
} //namespace dbpng

#endif


#endif

