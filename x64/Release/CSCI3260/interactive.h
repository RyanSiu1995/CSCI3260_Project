#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "resourceLoader.h"

GLfloat xMouse = 0.0f;
GLfloat yMouse = 0.0f;
GLfloat zMouse = 0.0f;
GLfloat mouse = 0.02f;
GLfloat lastx = 512;
GLfloat lasty = 256;
GLfloat camLocation_x = 0.0f;
GLfloat camLocation_y = 0.0f;
GLfloat camLocation_z = 0.0f;
GLfloat camTarget_x = 1.0f;
GLfloat camTarget_y = 0.0f;
GLfloat camTarget_z = 0.0f;
GLfloat camUp_x = 0.0f;
GLfloat camUp_y = 1.0f;
GLfloat camUp_z = 0.0f;
int trueUniverse = 0;
char position = 'o';
int spaceshipOnEarth = 1;
int spaceTravel = 0;
GLfloat spaceTravelInProgress = 0;
int spaceCam = 0;
GLfloat spaceshipSpeed = 1;

void move(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		if(spaceshipSpeed < 5.0f)
			spaceshipSpeed += 0.5f;
	}
	if (key == GLUT_KEY_DOWN) {
		if(spaceshipSpeed > 0.5f)
			spaceshipSpeed -= 0.1f;
	}
	if (key == GLUT_KEY_RIGHT) {
		xMouse -= 1.0f;
	}
	if (key == GLUT_KEY_LEFT) {
		xMouse += 1.0f;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == '1') {
		if (trueUniverse == 1) {
			trueUniverse = 0;
		}
		else {
			trueUniverse = 1;
		}
	}
	if (key == 27) {
		exit(0);
	}
	if (key == 'r') {
		camLocation_x = 0.0f;
		camLocation_y = 0.0f;
		camLocation_z = 0.0f;
		camTarget_x = 1.0f;
		camTarget_y = 0.0f;
		camTarget_z = 0.0f;
		camUp_x = 0.0f;
		camUp_y = 1.0f;
		camUp_z = 0.0f;
		xMouse = 0.0f;
		yMouse = 0.0f;
		zMouse = 0.0f;
		position = 'o';
	}
	if (key == 'a') {
		spaceCam = 0;
		camLocation_x = 10.0f;
		camLocation_y = 100.0f;
		camLocation_z = 0.0f;
		camTarget_x = 20.0f;
		camTarget_y = 0.0f;
		camTarget_z = 0.0f;
		camUp_x = 1.0f;
		camUp_y = 0.0f;
		camUp_z = 0.0f;
		xMouse = 0.0f;
		yMouse = 0.0f;
		zMouse = 0.0f;
		position = 'a';
	}
	if (key == 's') {
		spaceCam = 0;
		camLocation_x = 50.0f;
		camLocation_y = 20.0f;
		camLocation_z = 50.0f;
		camTarget_x = 50.0f;
		camTarget_y = 0.0f;
		camTarget_z = 0.0f;
		camUp_x = 0.0f;
		camUp_y = 1.0f;
		camUp_z = 0.0f;
		xMouse = 0.0f;
		yMouse = 0.0f;
		zMouse = 0.0f;
		position = 's';
	}
	if (key == 'd') {
		spaceCam = 0;
		camLocation_x = 50.0f;
		camLocation_y = 20.0f;
		camLocation_z = -50.0f;
		camTarget_x = 50.0f;
		camTarget_y = 0.0f;
		camTarget_z = 0.0f;
		camUp_x = 0.0f;
		camUp_y = 1.0f;
		camUp_z = 0.0f;
		xMouse = 0.0f;
		yMouse = 0.0f;
		zMouse = 0.0f;
		position = 'd';
	}
	if (key == 'T') {
			spaceTravel = 1;
			spaceTravelInProgress;
	}
	if (key == 'z') {
		if (spaceCam == 1) {
			spaceCam = 0;
			keyboard('r', 1, 1);
		}
		else {
			keyboard('r', 1, 1);
			spaceCam = 1;
		}
	}
		
}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation
	if (position == 'o' || position == 'a') {
		if (x > lastx)
			xMouse += mouse*abs(x - lastx);
		else
			xMouse -= mouse*abs(lastx - x);
		if (y > lasty)
			yMouse += mouse*abs(y - lasty);
		else
			yMouse -= mouse*abs(lasty - y);
	}
	else
	if(position == 's' || position == 'd'){
		if (x > lastx)
			zMouse += mouse*abs(x - lastx);
		else
			zMouse -= mouse*abs(lastx - x);
		if (y > lasty)
			yMouse += mouse*abs(y - lasty);
		else
			yMouse -= mouse*abs(lasty - y);
	}
	lastx = x;
	lasty = y;

}

void mouse_scroll(int button, int state, int x, int y) {
	if (button == 3)
		if (position == 'o')
			zMouse += 0.1f;
		else
		if (position == 'a')
			yMouse += 0.1f;
		else
		if(position == 's')
			xMouse -= 0.1f;
		else
		if (position == 'd')
			xMouse += 0.1f;
		
	if (button == 4)
		if (position == 'o')
			zMouse -= 0.1f;
		else
		if (position == 'a')
			yMouse -= 0.1f;
		else
		if (position == 's')
			xMouse += 0.1f;
		else
		if (position == 'd')
			xMouse -= 0.1f;
}