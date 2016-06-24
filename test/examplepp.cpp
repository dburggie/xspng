#include <xspng.h>

using namespace xspng;



int main() {
	int w = 256, h = 256;

	Image img(w,h);
	Color c;

	for (int y = 0; y < h; y++)
	for (int x = 0; x < w; x++) {
		c.r = (4 * x) % 256;
		c.g = (2 * y) % 256;
		c.b = (x + y) % 256;
		img.setPixel(x,y,c);
	}

	img.write("examplepp.png");

	return 0;
}



