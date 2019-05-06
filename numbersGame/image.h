#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#ifndef IMAGE_H
#define IMAGE_H
struct RGB
{
unsigned char red;
unsigned char green;
unsigned char blue;
};

class Image
{
private:
RGB pixels[2048][2048];
int width ;
int height ;
public :
	Image (std::string);
	void display (int , int );
};
#endif