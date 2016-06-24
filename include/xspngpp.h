
#ifndef __XSPNGPP_H
#define __XSPNGPP_H

/* xspngpp.h 
 *
 * Licensed: MIT
 * authored: Duncan Burggraf
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Duncan Burggraf
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <xspng.h>

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

#endif

