
#include <xspng.h>
#include <cassert> //assert
#include <cstring> //memcpy

using namespace xspng;

Color::Color() {
	r = 0x00;
	g = 0x00;
	b = 0x00;
	a = 0xff;
}



Color::Color(unsigned char r, unsigned char g, unsigned char b) {
	this->r = 0xff & r;
	this->g = 0xff & g;
	this->b = 0xff & b;
	this->a = 0xff;
}



Color::Color(unsigned int r, unsigned int g, unsigned int b) {
	this->r = 0xff & r;
	this->g = 0xff & g;
	this->b = 0xff & b;
	this->a = 0xff;
}



Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	this->r = 0xff & r;
	this->g = 0xff & g;
	this->b = 0xff & b;
	this->a = 0xff & a;
}




Color::Color(double r, double g, double b) {
	assert(0.0 <= r);
	assert(0.0 <= g);
	assert(0.0 <= b);
	assert(r < 1.0);
	assert(g < 1.0);
	assert(b < 1.0);
	this->r = 0xff & ((unsigned int) (256 * r));
	this->g = 0xff & ((unsigned int) (256 * g));
	this->b = 0xff & ((unsigned int) (256 * b));
	this->a = 0xff;
}



Color::Color(double r, double g, double b, double a) {
	assert(0.0 <= r);
	assert(0.0 <= g);
	assert(0.0 <= b);
	assert(0.0 <= a);
	assert(r < 1.0);
	assert(g < 1.0);
	assert(b < 1.0);
	assert(a < 1.0);
	this->r = 0xff & ((unsigned int) (256 * r));
	this->g = 0xff & ((unsigned int) (256 * g));
	this->b = 0xff & ((unsigned int) (256 * b));
	this->b = 0xff & ((unsigned int) (256 * a));
}




Image::Image() {
	width = height = 0;
	imgp = NULL;
}



Image::Image(unsigned int width, unsigned int height) {
	this->width = this->height = 0;
	imgp = NULL;
	setSize(width,height);
}



Image::Image(const Image & image) {
	width = height = 0;
	imgp = NULL;
	setSize(image.width, image.height);
	
	if (image.imgp && image.imgp->buffer) {
		imgp = xspng_image_new(image.width, image.height);
		memcpy(imgp->buffer, image.imgp->buffer, image.imgp->size);
	}
}



Image::~Image() {
	if (imgp) xspng_image_free(imgp);
}



void Image::setSize(unsigned int width, unsigned int height) {
	assert(0 < width);
	assert(0 < height);
	this->width = width;
	this->height = height;
	if (imgp) {
		xspng_image_free(imgp);
		imgp = NULL;
	}
	imgp = xspng_image_new(width, height);
	assert(imgp);
	assert(imgp->buffer);
}



void Image::setPixel(unsigned int x, unsigned int y, Color c) {
	assert(imgp != NULL);
	assert(imgp->buffer != NULL);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < width);
	assert(y < height);
	
	xspng_image_set_rgba(imgp,x,y,c.r,c.g,c.b,c.a);
}



void Image::setPixel(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b) {
	assert(imgp != NULL);
	assert(imgp->buffer != NULL);
	assert(0 <= x);
	assert(0 <= y);
	assert(x < width);
	assert(y < height);
	
	xspng_image_set_rgba(imgp,x,y,r,g,b,0xff);
}



void Image::write(const char * filename) {
	assert(0 < width);
	assert(0 < height);
	assert(imgp != NULL);
	assert(imgp->buffer != NULL);
	assert(filename != NULL);
	
	xspng_image_write(imgp,filename);
}







