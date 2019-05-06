// graphics.cpp : Defines the entry point for the console application.
//

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

using namespace std;


Image *imgs[12];

void loadImages()
{
	imgs[0] = new Image("1.bmp");
	imgs[1] = new Image("2.bmp");
	imgs[2] = new Image("3.bmp");
	imgs[3] = new Image("4.bmp");
	imgs[4] = new Image("5.bmp");
	imgs[5] = new Image("6.bmp");
	imgs[6] = new Image("7.bmp");
	imgs[7] = new Image("8.bmp");
	imgs[8] = new Image("9.bmp");
	imgs[9] = new Image("10.bmp");
	imgs[10] = new Image("11.bmp");
	imgs[11] = new Image("12.bmp");
}

struct point {
	int x, y;
	point() {}
	point(int x, int y) :x(x), y(y) {}
};

int NUMBER = 1;
int evenCounter = 0, oddCounter = 0;


struct Obj {
	int type;
	float h, v;
	int xDirection = 1;
	int yDirection = 1;
	int movmentSpeed;
	int stepsLimit = 0;
	vector<point> points;
	Obj(){
		h = 0;
		v = 0;
	}
	Obj(int t){
		h = 0;
		v = 0;
		type = t;
	}
	void AddPoint(point &p){
		points.push_back(p);
	}

	bool checkWithBoundaries()
	{
		for (int i = 0; i < 4; i++)
		{
			if (points[i].y == -250)
			{
				return false;
			}
		}
		return true;
	}

	vector<double> getNewBoundary(int nV, int nH) {
		vector<double> bound{ 1e7, -1e7, 1e7, -1e7 };
		for (int i = 0; i < points.size(); i++){
			bound[0] = min(bound[0], points[i].x + h + nH);
			bound[1] = max(bound[1], points[i].x + h + nH);
			bound[2] = min(bound[2], points[i].y + v + nV);
			bound[3] = max(bound[3], points[i].y + v + nV);
		}
		return bound;
	}

	void Draw() {
		glBegin(type);
		for (int i = 0; i < points.size(); i++){
			glVertex2f(points[i].x + h, points[i].y + v);
		}
		glEnd();
	}
};

Obj player;
vector<Obj> background;
vector<Obj> movingObjects;
vector<Obj> players;
vector<Obj> rotatingBack;

float bound[] = { -250, 250, -250, 250 };
char time[120] = { "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960" };
int k = 0;
bool WIN = 0;

void myStyleInit()
{
	glClearColor(0.9, 0.9, 0.9, 0.0);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glOrtho(bound[0], bound[1], bound[2], bound[3], 0, -1);
}

//TEXT
void drawBitmapText(char *car, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = car; *c != '\n'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void drawBitmapTime(char *car, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = car; *c != '\n'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

void drawBitmapCountDown(char *car, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);

	if (k > 9){
		if (k % 2 == 0){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, car[k]);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, car[k + 1]);
		}
		else{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, car[k + 1]);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, car[k + 2]);
		}
	}
	else
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, car[k]);
}

void render()
{
	glColor3f(0, 0, 0);
	glLoadIdentity();
	drawBitmapText("Numbers\n", -90, 200, 0); 
	drawBitmapText("Game\n", 15, 200, 0);
	drawBitmapText("EVEN\n", -170, 0, 0);
	drawBitmapText("ODD\n", 110, 0, 0);
	drawBitmapTime("Time: \n", 180, 220, 0);
	drawBitmapTime("Right click To\n", 135, 210, 0);
	drawBitmapTime("choose a level\n", 135, 200, 0);


	if (k >= 110 && !WIN){
		PlaySound(TEXT("Sound Oooooh!!.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		drawBitmapText("GAME  OVER\n", -70,60, 0);
	}
	else if (k<110)
		drawBitmapCountDown(time, 205, 220, 0);
	else if (k >= 110 && WIN){
		glColor3f(1, 0, 0);
		drawBitmapText("CONGRATULATION\n", -220, 110, 0);
		drawBitmapText("YOU HAVE WON\n", 50, 110, 0);
		PlaySound(TEXT("Sound Go go go.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
		
	glutSwapBuffers();
}


int v;
int h;
int ceta = 0;

void myDisplay()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColor3f(0.9, 0.9, 0.9);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(4.0);
	glLoadIdentity();

	glColor3f(0.047, 0.145, 0.46);
	for (int i = 0; i < background.size(); i++){
		background[i].Draw();
	}
	glColor3f(0.24, 0.75, 0.09);
	for (int i = 0; i < players.size(); i++){
		players[i].Draw();
	}

	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < movingObjects.size(); i++){	
		if (i % 2 == 0)
			imgs[i]->display(movingObjects[i].points[0].x + movingObjects[i].h, movingObjects[i].points[0].y + movingObjects[i].v);
		else
			imgs[i]->display(movingObjects[i].points[1].x + movingObjects[i].h, movingObjects[i].points[1].y + movingObjects[i].v);
	}

	
	glColor3f(0.87, 0.77, 0.87);
	for (int i = 0; i < rotatingBack.size(); i++){
		
		if (i < 2 && (NUMBER == 2 || NUMBER == 4)){
			glPushMatrix();
			glTranslatef(125, -120, 0);
			glRotatef(ceta, 0, 0, 1);
			glTranslatef(-125, 120, 0);
			rotatingBack[i].Draw();
			glPopMatrix();
		}
		if (i >= 2 && i<4 && (NUMBER == 2 || NUMBER == 4))
		{
			glPushMatrix();
			glTranslatef(125, 150, 0);
			glRotatef(ceta, 0, 0, 1);
			glTranslatef(-125, -150, 0);
			rotatingBack[i].Draw();
			glPopMatrix();

		}

		else if (i >= 4 && i<6 && (NUMBER == 3 || NUMBER == 4))
		{
			glPushMatrix();
			glTranslatef(-125, 150, 0);
			glRotatef(ceta, 0, 0, 1);
			glTranslatef(125, -150, 0);
			rotatingBack[i].Draw();
			glPopMatrix();
		}
		else if (i >= 6 && i<8 && (NUMBER == 3 || NUMBER == 4))
		{
			glPushMatrix();
			glTranslatef(-125, -120, 0);
			glRotatef(ceta, 0, 0, 1);
			glTranslatef(125, 120, 0);
			rotatingBack[i].Draw();
			glPopMatrix();
		}
	}
	render();
	glutSwapBuffers();
	//glFlush();

}


void Rotate(int val)
{
	ceta += 1;
	glutPostRedisplay();
	glutTimerFunc(25, Rotate, 0);
}

int startingNode = 0;
int startingDirection = 0;

void InsertShapes() {

	player.type = GL_POLYGON;
	player.AddPoint(point(-10, -430));
	player.AddPoint(point(10, -430));
	player.AddPoint(point(10, -50));
	player.AddPoint(point(-10, -50));
	player.stepsLimit = 150;

	Obj player1;
	player1.type = GL_POLYGON;
	player1.AddPoint(point(-10, 50));
	player1.AddPoint(point(10, 50));
	player1.AddPoint(point(10, 430));
	player1.AddPoint(point(-10, 430));
	player1.stepsLimit = 150;

	
	Obj southObstacle;
	southObstacle.type = GL_POLYGON;
	southObstacle.AddPoint(point(10.1, -230));
	southObstacle.AddPoint(point(250, -230));
	southObstacle.AddPoint(point(250, -250));
	southObstacle.AddPoint(point(10.1, -250));
	background.push_back(southObstacle);

	Obj southObstacle1;
	southObstacle1.type = GL_POLYGON;
	southObstacle1.AddPoint(point(-250, -230));
	southObstacle1.AddPoint(point(-10.1, -230));
	southObstacle1.AddPoint(point(-10.1, -250));
	southObstacle1.AddPoint(point(-250, -250));
	background.push_back(southObstacle1);

	Obj westObstacle;
	westObstacle.type = GL_POLYGON;
	westObstacle.AddPoint(point(-250, -250));
	westObstacle.AddPoint(point(-250, 250));
	westObstacle.AddPoint(point(-230, 250));
	westObstacle.AddPoint(point(-230, -250));
	background.push_back(westObstacle);

	Obj northObstacle;
	northObstacle.type = GL_POLYGON;
	northObstacle.AddPoint(point(-250, 230));
	northObstacle.AddPoint(point(-10.1, 230));
	northObstacle.AddPoint(point(-10.1, 250));
	northObstacle.AddPoint(point(-250, 250));
	background.push_back(northObstacle);

	Obj northObstacle1;
	northObstacle1.type = GL_POLYGON;
	northObstacle1.AddPoint(point(10.1, 230));
	northObstacle1.AddPoint(point(250, 230));
	northObstacle1.AddPoint(point(250, 250));
	northObstacle1.AddPoint(point(10.1, 250));
	background.push_back(northObstacle1);

	Obj EastObstacle;
	EastObstacle.type = GL_POLYGON;
	EastObstacle.AddPoint(point(250, -250));
	EastObstacle.AddPoint(point(250, 250));
	EastObstacle.AddPoint(point(230, 250));
	EastObstacle.AddPoint(point(230, -250));
	background.push_back(EastObstacle);
	
	//Rotating Objects
	Obj rotating1;
	rotating1.type = GL_POLYGON;
	rotating1.AddPoint(point(50, -145));
	rotating1.AddPoint(point(200, -145));
	rotating1.AddPoint(point(200, -95));
	rotating1.AddPoint(point(50, -95));
	rotatingBack.push_back(rotating1);

	Obj rotating2;
	rotating2.type = GL_POLYGON;
	rotating2.AddPoint(point(100, -195));
	rotating2.AddPoint(point(150, -195));
	rotating2.AddPoint(point(150, -45));
	rotating2.AddPoint(point(100, -45));
	rotatingBack.push_back(rotating2);


	Obj rotating1A;
	rotating1A.type = GL_POLYGON;
	rotating1A.AddPoint(point(50, 125));
	rotating1A.AddPoint(point(200, 125));
	rotating1A.AddPoint(point(200, 175));
	rotating1A.AddPoint(point(50, 175));
	rotatingBack.push_back(rotating1A);

	Obj rotating2A;
	rotating2A.type = GL_POLYGON;
	rotating2A.AddPoint(point(100, 75));
	rotating2A.AddPoint(point(150, 75));
	rotating2A.AddPoint(point(150, 225));
	rotating2A.AddPoint(point(100, 225));
	rotatingBack.push_back(rotating2A);

	Obj rotating3;
	rotating3.type = GL_POLYGON;
	rotating3.AddPoint(point(-50, 125));
	rotating3.AddPoint(point(-200, 125));
	rotating3.AddPoint(point(-200, 175));
	rotating3.AddPoint(point(-50, 175));
	rotatingBack.push_back(rotating3);

	Obj rotating4;
	rotating4.type = GL_POLYGON;
	rotating4.AddPoint(point(-100, 75));
	rotating4.AddPoint(point(-150, 75));
	rotating4.AddPoint(point(-150, 225));
	rotating4.AddPoint(point(-100, 225));
	rotatingBack.push_back(rotating4);

	Obj rotating3A;
	rotating3A.type = GL_POLYGON;
	rotating3A.AddPoint(point(-50, -145));
	rotating3A.AddPoint(point(-200, -145));
	rotating3A.AddPoint(point(-200, -95));
	rotating3A.AddPoint(point(-50, -95));
	rotatingBack.push_back(rotating3A);

	Obj rotating4A;
	rotating4A.type = GL_POLYGON;
	rotating4A.AddPoint(point(-100, -195));
	rotating4A.AddPoint(point(-150, -195));
	rotating4A.AddPoint(point(-150, -45));
	rotating4A.AddPoint(point(-100, -45));
	rotatingBack.push_back(rotating4A);

	int x1 = -220, x2 = -200, y1 = 190, y2 = 210;
	int startingX = 1, startingY = 1;

	for (int i = 0; i < NUMBER; i++)
	{
		Obj movingObj;
		movingObj.type = GL_POLYGON;
		movingObj.AddPoint(point(x1, y1));
		movingObj.AddPoint(point(x2, y1));
		movingObj.AddPoint(point(x2, y2));
		movingObj.AddPoint(point(x1, y2));
		movingObj.xDirection = startingX;
		movingObj.yDirection = startingY;
		movingObj.movmentSpeed = 1;
		movingObjects.push_back(movingObj);


		Obj movingObj2;
		movingObj2.type = GL_POLYGON;
		movingObj2.AddPoint(point(-x1, y1));
		movingObj2.AddPoint(point(-x2, y1));
		movingObj2.AddPoint(point(-x2, y2));
		movingObj2.AddPoint(point(-x1, y2));
		movingObj.xDirection = startingX;
		movingObj.yDirection = startingY;
		movingObj2.movmentSpeed = 1;
		movingObjects.push_back(movingObj2);

		startingX *= -1;
		startingY *= -1;

		if (i == 6)
		{
			x1 += 50;
			x2 += 50;
			y1 += 50;
			y2 += 50;
		}
		else if (i > 6)
		{
			y1 += 50;
			y2 += 50;
		}
		else
		{
			y1 -= 50;
			y2 -= 50;
		}
	}

	background.push_back(player);
	background.push_back(player1);

}

void Reset() {
	for (int i = 0; i < movingObjects.size(); i++){
			movingObjects[i].h  = 0;
			movingObjects[i].v  = 0;
	}
}

#define IN  1
#define OUT 2
#define BOUNDARY  3
#define EPS 1e-9
#define same(a,b) fabs((a) - (b)) < EPS

int CollideX(vector<double> &s1, vector<double> &s2) {
	if (s1[0] > s2[0])
		return CollideX(s2, s1);
	if (same(s1[1], s2[0]))
		return BOUNDARY;
	if (s1[1] < s2[0])
		return OUT;
	return IN;
}

int CollideY(vector<double> &s1, vector<double> &s2) {
	if (s1[2] > s2[2])
		return CollideY(s2, s1);
	if (same(s1[3], s2[2]))
		return BOUNDARY;
	if (s1[3] < s2[2])
		return OUT;
	return IN;
}

bool Collide(vector<double> &s1, vector<double> &s2) {
	int CX = CollideX(s1, s2);
	int CY = CollideY(s1, s2);
	return (CX == IN && CY == IN);
}


//Handling the moving object obstacles
bool checkInBoundary_MovingObj(int ind, float v, float h) {
	vector<double> shapeB = movingObjects[ind].getNewBoundary(v, h);
	for (int i = 0; i < 4; i++){
		if (i % 2 == 0 && shapeB[i] < bound[i]
			|| i % 2 == 1 && shapeB[i] > bound[i]){
			return false;
		}
	}
	return true;
}

bool MovingObj_checkCollide(int ind, float v, float h) {
	vector<double> shapeP = movingObjects[ind].getNewBoundary(v, h);
	for (int i = 0; i < background.size(); i++) {
		vector<double> shapeB = background[i].getNewBoundary(0, 0);
		if (Collide(shapeP, shapeB)){
			return true;
		}
	}

	for (int i = 0; i < movingObjects.size(); i++) {
		if (i != ind){
			vector<double> shapeB = movingObjects[i].getNewBoundary(0, 0);
			if (Collide(shapeP, shapeB)){
				return true;
			}
		}
	}

	return false;
}

void CalculateScore()
{
	for (int i = 0; i < movingObjects.size(); i++)
	{ 
		WIN = 0;
		if (i % 2 == 0)
		{
			if (movingObjects[i].points[0].x + movingObjects[i].h > 0)
				oddCounter++;
		}
		else
		{
			if (movingObjects[i].points[0].x + movingObjects[i].h < 0)
				evenCounter++;
		}
	}
	if (oddCounter==NUMBER && evenCounter==NUMBER)
	{
		WIN = 1;
		k = 110;
	}
	oddCounter = 0;
	evenCounter = 0;
}


void MoveObj(int ind, float v, float h) {
	//	if (!checkInBoundary_MovingObj(ind, v, h)) return;
	if (MovingObj_checkCollide(ind, v, h)) return;
	movingObjects[ind].v += v;
	movingObjects[ind].h += h;
	CalculateScore();
	return;
}


bool CheckMove(int ind, float v, float h) {
	//if (!checkInBoundary_MovingObj(ind, v, h)) return false;
	if (MovingObj_checkCollide(ind, v, h)) return false;
	return true;
}


//Moving rod collision avoidance
bool MovingObj_checkCollideRod(int ind, float v, float h) {
	vector<double> shapeP = background[ind].getNewBoundary(v, h);

	for (int i = 0; i < movingObjects.size(); i++) {

		vector<double> shapeB = movingObjects[i].getNewBoundary(0, 0);
		if (Collide(shapeP, shapeB)) {
			movingObjects[i].yDirection = -1;
			movingObjects[i].xDirection = -1;
			MoveObj(i, 0, movingObjects[i].movmentSpeed * movingObjects[i].movmentSpeed * movingObjects[i].xDirection);
			MoveObj(i, movingObjects[i].movmentSpeed* movingObjects[i].movmentSpeed * movingObjects[i].yDirection, 0);
			return true;
		}
		//return false;
	}


	return false;
}

bool CheckMoveRod(int ind, float v, float h) {
	//if (!checkInBoundary_MovingObj(ind, v, h)) return false;
	if (MovingObj_checkCollideRod(ind, v, h)) return false;
	return true;
}

//end of moving rod collision avoidance

void CountDown(int val){
	if (k >= 110){
		Sleep(3000);
		Reset();
		Sleep(2000);
		PlaySound(TEXT("Mario.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		k = 0;
	}
	else if (k == 110){
		WIN = 1;
		Reset();
		Sleep(1000);
		PlaySound(TEXT("Mario.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		k = 0;
	}
	else
		k++;
		
	glutPostRedisplay();
	glutTimerFunc(1000, CountDown, val);
}

void myTimer(int val)
{
	for (val = 0; val < movingObjects.size(); val++)
	{
		if (!CheckMove(val, movingObjects[val].movmentSpeed, 0))
			movingObjects[val].yDirection = -1;
		else if (!CheckMove(val, - movingObjects[val].movmentSpeed , 0))
			movingObjects[val].yDirection = 1;


		if (!CheckMove(val, 0, movingObjects[val].movmentSpeed))
			movingObjects[val].xDirection = -1;
		else if (!CheckMove(val, 0, -movingObjects[val].movmentSpeed ))
			movingObjects[val].xDirection = 1;

		//printf("%d%d",movingObjects[val].xDirection, movingObjects[val].xDirection);
		MoveObj(val, movingObjects[val].movmentSpeed * movingObjects[val].yDirection, 0);
		MoveObj(val, 0, movingObjects[val].movmentSpeed * movingObjects[val].xDirection);
	}

	glutPostRedisplay();
	glutTimerFunc(10, myTimer, val);

}

void Move(int ind, int h, int v) {
	/*printf("%d %d\n", background[ind - 1].stepsLimit, background[ind - 2].stepsLimit);
	if (background[ind].stepsLimit > 0 && background[ind].stepsLimit < 270)
	{*/
	if (CheckMoveRod(ind, v, h)&&CheckMoveRod(ind-1,v,h))
	{
		background[ind].v += v;
		background[ind-1].v += v;
	}
	
	/*}

	if (background[ind].stepsLimit <= -10)
	{
		background[ind].stepsLimit = 0;
	}

	if (background[ind].stepsLimit >= 270)
	{
		background[ind].stepsLimit = 270;
	}
	printf("Player1: %d Player: %d !\n", background[ind - 1].stepsLimit, background[ind - 2].stepsLimit);
	*/
}

void specialInput(int key, int x, int y)
{
	int ind = background.size();
	switch (key)
	{
	case GLUT_KEY_UP:
		if (background[ind - 2].points[2].y + background[ind - 2].v + 10 <= 130)
		{
			Move(ind - 1, 0, 10);
		}

		break;
	case GLUT_KEY_DOWN:
		if (background[ind - 1].points[0].y + background[ind - 1].v + 10 >= -110)
		{
			Move(ind - 1, 0, -10);
		}

		break;
	}
	myDisplay();
}

void keyboard(unsigned char ch, int x, int y)
{
	if (ch == 'w') {
		int ind = background.size();
		if (background[ind - 2].points[2].y + background[ind - 2].v + 10 <= 130 )
			Move(ind-1, 0, 10);
	}
	else if (ch == 's')
	{
		int ind = background.size();
		if (background[ind - 1].points[0].y + background[ind - 1].v + 10 >= -110)
			Move(ind - 1, 0, -10);
	}
	if (ch == 'q')
		exit(0);
	
	myDisplay();
}


void ClearShapes()
{
	background.clear();
	movingObjects.clear();
	players.clear();
	rotatingBack.clear();

}


void SelectLevel(GLint selectedLevel)
{
	switch (selectedLevel){
	case 1:
		NUMBER = 1;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;
	case 2:
		NUMBER = 2;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;

	case 3:
		NUMBER = 3;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;

	case 4:
		NUMBER = 4;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;

	case 5:
		NUMBER = 5;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;

	case 6:
		NUMBER = 6;
		k = 0;
		Reset();
		ClearShapes();
		InsertShapes();
		break;

	}

	glutPostRedisplay();
}

int Secondmain()
{
	myStyleInit();
	InsertShapes();
	loadImages();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialInput);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(10, myTimer, 0);

	PlaySound(TEXT("Mario.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	if (k < 110)
		glutTimerFunc(1000, CountDown, 1);
	
	glutTimerFunc(80, Rotate, 0);
		
	glutCreateMenu(SelectLevel);
		glutAddMenuEntry("Level 1", 1);
		glutAddMenuEntry("Level 2", 2);
		glutAddMenuEntry("Level 3", 3);
		glutAddMenuEntry("Level 4", 4);
		glutAddMenuEntry("Level 5", 5);
		glutAddMenuEntry("Level 6", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutMainLoop();
	return 0;
}

