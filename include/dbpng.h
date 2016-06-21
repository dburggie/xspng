
#ifndef __DBPNG_H
#define __DBPNG_H

#include <stdio.h>
#include <stdint.h>

/* *
 *
 * Using this library:
 * 
 * Declare a dbpng_imagep variable as so:
 *
 *		#	dbpng_int width = 256, height = 256; //four byte integer type
 *		#	dbpng_imagep imgp = dbpng_new_image(width,height);
 *
 * Iterate through x and y values ((0,0) is the upper-left-most pixel), setting
 * image sample values to the desired values.
 *
 *		#	dbpng_byte r, g, b; //1 byte unsigned integer type
 *		#	dbpng_int x,y;
 *		#	for (y = 0; y < height; y++) {
 *		#		for (x = 0; x < width; x++) {
 *		#			r = 0xff & x;
 *		#			g = 0xff & y;
 *		#			b = 0xff & ((x + y) / 2);
 *		#			dbpng_set_rgb(imgp,x,y,r,g,b);
 *		#		}
 *		#	}
 *
 *	Write the image to file
 *
 *		#	dbpng_write_to_file(imgp, "output.png");
 *
 * */




/* don't mangle our declarations with a C++ compiler */
#ifdef __cplusplus
namespace dbpng {
	extern "C" {
#endif /* __CPLUSPLUS */



		typedef uint8_t  dbpng_byte;
		typedef uint32_t dbpng_int;

		//primary image data structure
		typedef struct {
			dbpng_int width, height;
			dbpng_byte * buffer;
		} dbpng_image;

		//pointer to the above-defined struct
		typedef dbpng_image* dbpng_imagep;

		//struct initialization functions
		dbpng_imagep dbpng_new_image(dbpng_int width, dbpng_int height);
		void dbpng_free_image(dbpng_imagep imgp);
		void dbpng_image_init(dbpng_image *img);

		//set a pixel to the given red-green-blue samples
		void dbpng_set_rgb(
				dbpng_imagep imgp, 
				dbpng_int x, dbpng_int y, 
				dbpng_byte r, dbpng_byte g, dbpng_byte b
			);

		//set a pixel to the given red-green-blue-alpha samples
		void dbpng_set_rgba(
				dbpng_imagep imgp,
				dbpng_int x, dbpng_int y,
				dbpng_byte r, dbpng_byte g, dbpng_byte b, dbpng_byte a
			);

		//write a dbpng_image struct to file
		void dbpng_write_to_file(dbpng_imagep, const char * filename);



/* end anti-prototype-mangling protections */
#ifdef __cplusplus
	} // extern "C"

	class Color {
		public:
			dbpng_byte r, g, b, a;

			Color();
			Color(int r, int g, int b);
			Color(int r, int g, int b, int a);
			Color(dbpng_byte r, dbpng_byte g, dbpng_byte b);
			Color(dbpng_byte r, dbpng_byte g, dbpng_byte b, dbpng_byte a);
			Color(double r, double g, double b);
			Color(double r, double g, double b, double a);
	};

	class Image {
		private:
			dbpng_int width, height;
			dbpng_imagep imgp;
		public:
			Image();
			Image(int width, int height);
			Image(dbpng_int width, dbpng_int height);
			~Image();

			void setSize(int width, int height);
			void setSize(dbpng_int width, dbpng_int height);

			void setPixel(int x, int y, Color c);

			void write(const char * filename);
	};
} // namespace dbpng
#endif /* __CPLUSPLUS */



#endif /* __DBPNG_H */

