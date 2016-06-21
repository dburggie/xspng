# dbpng

a simple library for drawing png files

### Motivation

For various reasons, I've decided to reinvent the wheel when it comes to
writing png files. Primarily, I've found version differences within libpng
stopping my code's ability to be compiled on different linux computers. It's
also a pretty fun learning experience.

### How to Use It

`dbpng` provides a simple api for drawing png files. It assumes you want an
`rgb` or an `rbga` colored image, and that you want your image interlaced.
`dbpng` does not allow you to change these settings for ease of use (and
implementation). When you `#include <dbpng.h>` in a C++ context, all the
functions are dropped into the `dbpng` namespace.

#### Typedefs

Png files use 32 bit integers and 8 bit characters. Integers only use the
least significant 31 bits, though. We define these types for you as `dbpng_int`
and `dbpng_byte`. The image data sits in a struct of type `dbpng_image`, though
I recommend using the pointer type `dbpng_imagep`.

#### The API

The only functions you should need are the following:

    //allocate a new dbpng_image of with the right buffer size
    dbpng_imagep dbpng_new_image(dbpng_int width, dbpng_int height);
    
    //free a dbpng_imagep and it's buffer
    void dbpng_free_image(dbpng_imagep img);
    
    //write a dbpng_imagep to file
    void dbpng_write_to_file(dbpng_imagep img, const char * filename);
    
    //set a pixel to the given rgb sample values
    void dbpng_set_rgb(
        dbpng_imagep img,
        dbpng_int x, dbpng_int y,
        dbpng_byte r, dbpng_byte g, dbpng_byte b
      );
    
    //set a pixel to the given rgba sample values
    void dbpng_set_rgba(
        dbpng_imagep img,
        dbpng_int x, dbpng_int y,
        dbpng_byte r, dbpng_byte g, dbpng_byte b, dbpng_byte a
      );

#### Example code

    #include <dbpng.h>
    
    int main() {
      
      //set width and height
      dbpng_int w = 256, h = 256;
      
      //allocate image buffer
      dbpng_imagep imgp = dbpng_new_image(w,h);
      
      //iterate through x,y coordinates and set rgb values
      dbpng_int x,y;
      dbpng_byte r,g,b;
      for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
          r = 0xff & x;
          g = 0xff & y;
          b = 0xff & ((x + y) / 2)
          dbpng_set_pixel(imgp,x,y,r,g,b);
        }
      }
      
      //write to file
      dbpng_write_to_file(imgp, "output.png");
      
      return 0;
    }
