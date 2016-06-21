
#ifndef __DBPNG_PRIVATE_H
#define __DBPNG_PRIVATE_H

#include <dbpng.h>
#include <stdio.h>

#ifdef __cplusplus
namespace dbpng {
	extern "C" {
#endif
		typedef struct {
			dbpng_byte sig[4];
			dbpng_int length;
			dbpng_byte *buffer;
			dbpng_int crc;
		} dbpng_chunk;

		typedef dbpng_chunk* dbpng_chunkp;

		void dbpng_write_file_sig(FILE* fout);
		void dbpng_write_int(FILE* fout, dbpng_int i);
		void dbpng_write_byte(FILE* fout, dbpng_byte b);
		void dbpng_write_chunk(FILE* fout, dbpng_chunkp chunk);

		dbpng_int dbpng_calc_crc(dbpng_chunkp chunk);
		void dbpng_chunk_deflate(dbpng_chunkp chunk);

		dbpng_chunkp dbpng_make_ihdr(dbpng_imagep img);
		dbpng_chunkp dbpng_make_idat(dbpng_imagep img);
		dbpng_chunkp dbpng_make_iend();

		void dbpng_free_chunk(dbpng_chunkp chunk);

#ifdef __cplusplus
	} //extern "C"
} //namespace dbpng

#endif


#endif

