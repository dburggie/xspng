
#include <xspng.h>
#include <assert.h>


int main() {
	
	xspng_byte r,g,b;
	xspng_int w = 512, h = 512;

	xspng_imagep img = xspng_image_new(w,h);
	assert(img != NULL);
	assert(img->width == w);
	assert(img->height == h);
	
	int x,y;

	for (y = 0; y < h; y++)
	for (x = 0; x < w; x++) {
		r = 0xff & ((x + y) / 4);
		g = 0xff & (x / 2);
		b = 0xff & (y / 2);
		xspng_image_set_rgb(img,x,y,r,g,b);
	}

	xspng_image_write(img, "bullshit.png");

	xspng_image_free(img);

	return 0;
}


