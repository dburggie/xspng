
#ifndef __XSPNG_H
#define __XSPNG_H

#include <stdio.h>
#include <stdint.h>

/* *
 *
 * Using this library:
 * 
 * Declare a xspng_imagep variable as so:
 *
 *		#	xspng_int width = 256, height = 256; //four byte integer type
 *		#	xspng_imagep imgp = xspng_new_image(width,height);
 *
 * Iterate through x and y values ((0,0) is the upper-left-most pixel), setting
 * image sample values to the desired values.
 *
 *		#	xspng_byte r, g, b; //1 byte unsigned integer type
 *		#	xspng_int x,y;
 *		#	for (y = 0; y < height; y++) {
 *		#		for (x = 0; x < width; x++) {
 *		#			r = 0xff & x;
 *		#			g = 0xff & y;
 *		#			b = 0xff & ((x + y) / 2);
 *		#			xspng_set_rgb(imgp,x,y,r,g,b);
 *		#		}
 *		#	}
 *
 *	Write the image to file
 *
 *		#	xspng_write_to_file(imgp, "output.png");
 *
 * */




/* don't mangle our declarations with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



	typedef uint8_t  xspng_byte;
	typedef uint32_t xspng_int;

	//primary image data structure
	typedef struct {
		xspng_int width, height, stride, size;
		xspng_byte * buffer;
	} xspng_image;

	//pointer to the above-defined struct
	typedef xspng_image* xspng_imagep;

	//struct initialization functions
	xspng_imagep xspng_image_new(xspng_int width, xspng_int height);
	void xspng_image_free(xspng_imagep imgp);
	void xspng_image_init(xspng_image *img);

	//set a pixel to the given red-green-blue samples
	void xspng_image_set_rgb(
			xspng_imagep imgp, 
			xspng_int x, xspng_int y, 
			xspng_byte r, xspng_byte g, xspng_byte b
		);

	//set a pixel to the given red-green-blue-alpha samples
	void xspng_image_set_rgba(
			xspng_imagep imgp,
			xspng_int x, xspng_int y,
			xspng_byte r, xspng_byte g, xspng_byte b, xspng_byte a
		);

	//write a xspng_image struct to file
	void xspng_image_write(xspng_imagep, const char * filename);



/* end anti-prototype-mangling protections */
#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */



#endif /* __XSPNG_H */

