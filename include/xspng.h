

/* * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  xspng.h 
 *
 *  Licensed: MIT
 *  authored: Duncan Burggraf
 *
 * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016 Duncan Burggraf
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __XSPNG_H
#define __XSPNG_H

#include <stdio.h>
#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



	/* * * * * Typedefs * * * * */

	typedef uint8_t  xspng_byte;
	typedef uint32_t xspng_int;

	//primary image data structure
	typedef struct {
		xspng_int width, height, stride, size;
		xspng_byte * buffer;
	} xspng_image;

	//pointer to the above-defined struct
	typedef xspng_image* xspng_imagep;



	/* * * * * struct initialization functions * * * * */

	//return a pointer to a new image struct with allocated buffer
	xspng_imagep xspng_image_new(xspng_int width, xspng_int height);

	//free an image struct and it's buffer
	void xspng_image_free(xspng_imagep imgp);

	//free old buffer (if any) and allocate a new one
	void xspng_image_init(xspng_image *img);



	/* * * * * Pixel Setting Functions * * * * */

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



	/* * * * * Image Writing Functions * * * * */

	void xspng_image_write(xspng_imagep, const char * filename);



#ifdef __cplusplus
} 

namespace xspng {
	
	class Color {
		public:
			xspng_byte r, g, b, a;

			Color();
			Color(unsigned char r, unsigned char g, unsigned char b);
			Color(unsigned int r, unsigned int g, unsigned int b);
			Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
			Color(double r, double g, double b);
			Color(double r, double g, double b, double a);
	};

	class Image {
		private:
			xspng_int width, height;
			xspng_imagep imgp;
		public:
			Image();
			Image(unsigned int width, unsigned int height);
			Image(const Image & image);
			~Image();

			void setSize(unsigned int width, unsigned int height);

			void setPixel(unsigned int x, unsigned int y, Color c);
			void setPixel(unsigned int x, unsigned int y, 
					unsigned int r, unsigned int g, unsigned int b);

			void write(const char * filename);
	};
}
#endif /* __cplusplus */ 



#endif

