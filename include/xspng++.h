
#ifndef __XSPNGPP_H
#define __XSPNGPP_H

#include <xspng.h>

namespace xspng {
	
	class Color {
		public:
			xspng_byte r, g, b, a;

			Color();
			Color(unsigned char r, unsigned char g, unsigned char b);
			Color(int r, int g, int b);
			Color(int r, int g, int b, int a);
			Color(double r, double g, double b);
			Color(double r, double g, double b, double a);
	};

	class Image {
		private:
			xspng_int width, height;
			xspng_imagep imgp;
		public:
			Image();
			Image(int width, int height);
			~Image();

			void setSize(int width, int height);

			void setPixel(int x, int y, Color c);
			void setPixel(int x, int y, int r, int g, int b);

			void write(const char * filename);
	};
}

#endif