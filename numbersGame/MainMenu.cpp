#include "stdafx.h"
#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<vector>
#include <mmsystem.h>
#include "SOIL.h"
#include "image.h"
#include "MainMenu.h"

using namespace std;

Image *startGame;


float bound2[] = { -250, 250, -250, 250 };

void myStyleInit2()
{
	glClearColor(0.9, 0.9, 0.9, 0.0);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glOrtho(bound2[0], bound2[1], bound2[2], bound2[3], 0, -1);
}


void myDisplay2()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(0.9, 0.9, 0.9);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(4.0);
	glLoadIdentity();
	startGame = new Image("StartingMenu.bmp");
	startGame->display(-250, -250);
	glutSwapBuffers();
	//glFlush();
}



void keyboard2(unsigned char ch, int x, int y)
{
	if (ch == 13) {
		Secondmain();
	}
	else if (ch == 27){
		exit(0);
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Odd and Even");
	myStyleInit2();
	glutDisplayFunc(myDisplay2);
	glutKeyboardFunc(keyboard2);
	glutMainLoop();	
	return 0;
}