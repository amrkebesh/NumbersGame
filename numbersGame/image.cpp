#include "stdafx.h"
 #include <cstdlib>
 #include <cstdio>
 #include <cmath>
 #include <string>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
 #include  "image.h"

 Image :: Image ( std :: string filename )
 {
 FILE* image = fopen ( filename.c_str() , "rb" );
 unsigned char c ;
 for ( int i=0 ; i<18 ; i++)
 fscanf ( image , "%c" , &c );
 unsigned char b[4];
 fscanf ( image , "%c%c%c%c" , &b[0] , &b [1] , &b[2] , &b[3] );
 width = ( b[3] <<24) | ( b[2] <<16) | ( b[1] <<8) | b[0];
 fscanf ( image , "%c%c%c%c" , &b[0] , &b[1] , &b[2] , &b[3] );
 height = ( b[3] << 24) | ( b[2] << 16 ) | ( b[1] << 8 ) | b[0];
 for ( int i=0 ; i<28 ; i++)
 fscanf ( image , "%c" , &c ) ;
 for ( int i=0 ; i<height ; i++)
 {
 for ( int j=0; j<width ; j++)
 {
 char r , g , b ;
 fscanf ( image , "%c%c%c" , &b , &g , &r ) ;
 pixels[i][j].red = r ;
 pixels[i][j].green = g ;
 pixels[i][j].blue = b ;
 }
 if ( ( width*3 )%4 == 0 )
 continue ;
 for ( int j = 0 ; j< 4-(( width*3 )%4) ; j++)
 fscanf ( image , "%c", &c ) ;

 }
 fclose (image);
 }

 void Image :: display ( int win_x , int win_y )
 {
	glBegin (GL_POINTS);
	for ( int i = 0 , h = win_y ; i < height ; i++, h++)
	{
		for ( int j = 0 , w = win_x ; j < width ; j++, w++)
		{
			float r = ((float) pixels[i][j].red)/255.0;
			float g = ((float) pixels[i][j].green)/255.0;
			float b = ((float) pixels[i][j].blue)/255.0;
			glColor3f ( r , g , b );
			glVertex2i ( w , h );
		}
	}
 glEnd ( ) ;
}