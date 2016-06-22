
#include <zlib.h>



//use zlib to compress a chunk's buffer
//updates buffer size and length field
void xspng_chunk_deflate(xspng_chunkp chunk); //todo