
#ifndef __XSPNGPP_H
#define __XSPNGPP_H

#include <xspng.h>

namespace xspng {
	
	class Color {
		public:
			xspng_byte r, g, b, a;

			Color();
			Color(unsigned char r, unsigned char g, unsigned char b);
			Color(unsigned int r, unsigned int g, unsigned int b);
			Color(unsigned int r, 
					unsigned int g, unsigned int b, unsigned int a);
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
