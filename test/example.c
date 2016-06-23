#include <xspng.h>

int main() {
	  
	//set width and height
	xspng_int w = 256, h = 256;
	
	//allocate image buffer
	xspng_imagep imgp = xspng_image_new(w,h);
	
	//iterate through x,y coordinates and set rgb values
	xspng_int x,y;
	xspng_byte r,g,b;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			r = 0xff & x;
			g = 0xff & y;
			b = 0xff & ((x + y) / 2);
			xspng_image_set_rgb(imgp,x,y,r,g,b);
		}
	}
	
	//write to file
	xspng_image_write(imgp, "example.png");
	
	return 0;
}
