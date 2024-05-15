#include <iostream>
#include <tuple>
#include <vector>
#include <glut.h>
#include <windows.h>

#define _USE_MATH_DEFINES

#include <math.h>

#include "Saper.h"

struct Point3D {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Point3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

Point3D vertices[] = {
	{1,1,0},
	{1,-1,0},
	{-1,-1,0},
	{-1,1,0}
};

struct Point2D {
	float x = 0.0f;
	float y = 0.0f;
};

float pyramid[] = {
	0,0,2,
	1,1,0,
	1,-1,0,
	-1,-1,0,
	-1,1,0,
	1,1,0
};

Point2D pos = { 0, 0 };

float xAlfa = 20;
float zAlfa = 20;

void MoveCamera() {

	glRotatef(-xAlfa, 1, 0, 0);
	glRotatef(-zAlfa, 0, 0, 1);
	glTranslatef(-pos.x, -pos.y, -3);
}

void draw_circle() {

	std::vector<Point3D> circle_first;

	glColor3f(1, 0, 1);

	int cnt = 30;

	float x, y;
	float da = M_PI * 2.0 / cnt;

	circle_first.push_back(Point3D(0, 0, 0));

	//glVertex3f(0, 0, 0);
	for (int i = 0; i <= cnt; ++i) {
		x = sin(da * i);
		y = cos(da * i);
		circle_first.push_back(Point3D(0, x, y));
	}

	std::vector<Point3D> circle_second;

	//glVertex3f(0, 0, 0);
	for (int i = 0; i <= cnt; ++i) {
		x = sin(da * i);
		y = cos(da * i);
		circle_second.push_back(Point3D(5, x, y));
	}

	std::vector<Point3D> most;

	glColor3f(1, 1, 0);

	for (int i = 0, j = 0; i < circle_first.size() - 1 && j < circle_second.size(); ++i, ++j) {
		most.push_back(circle_first[i]);
		most.push_back(circle_second[j]);
	}

	std::vector<Point3D> tooth;

	for (int i = 1; i < circle_first.size() - 1; ++i) {
		Point3D left = circle_first[i - 1];
		Point3D right = circle_first[i];

		float delta_x = right.y - left.y;
		float delta_y = right.z - left.z;

		float angle = atan(delta_y / delta_x);

		tooth.push_back(left);
		tooth.push_back(right);
		tooth.push_back(Point3D(0, right.y - 0.5 * sin(angle), right.z - 0.5 * cos(angle)));

	}


	glVertexPointer(3, GL_FLOAT, 0, circle_first.data());
	glDrawArrays(GL_TRIANGLE_FAN, 0, circle_first.size());

	glVertexPointer(3, GL_FLOAT, 0, circle_second.data());
	glDrawArrays(GL_TRIANGLE_FAN, 0, circle_second.size());

	glVertexPointer(3, GL_FLOAT, 0, most.data());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, most.size());

	glVertexPointer(3, GL_FLOAT, 0, tooth.data());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, tooth.size());

}

void ShowWorld() {

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices);

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			glPushMatrix();

			if ((i + j) % 2 == 0)
				glColor3f(0, 0.5f, 0);
			else
				glColor3f(1, 1, 1);

			glTranslatef(i * 2, j * 2, 0);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glPopMatrix();
		}
	}
	/*
	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(1, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(1, 1, 0);
	glTranslatef(3, 3, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(0, 1, 0);
	glTranslatef(-3, 3, 0);
	glRotatef(45, 0, 0, 1);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glColor3f(0, 0.5, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	*/

	draw_circle();


	glDisableClientState(GL_VERTEX_ARRAY);
}

void display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	MoveCamera();
	ShowWorld();
	glPopMatrix();

	glutSwapBuffers();

	//Sleep(500);
}

void handleKeyPress(unsigned char key, int x, int y) {
	std::cout << "Key pressed: " << key << std::endl;

	if (key == 'i')
		xAlfa = ++xAlfa > 180 ? 180 : xAlfa;
	if (key == 'k')
		xAlfa = --xAlfa < 0 ? 0 : xAlfa;
	if (key == 'j')
		++zAlfa;
	if (key == 'l')
		--zAlfa;

	float ugol = -zAlfa / 180 * M_PI;
	float speed = 0.0f;

	if (key == 'w')
		speed = 0.1f;
	if (key == 's')
		speed = -0.1f;
	if (key == 'a') {
		speed = 0.1f;
		ugol -= M_PI * 0.5;
	}
	if (key == 'd') {
		speed = 0.1f;
		ugol += M_PI * 0.5;
	}

	if (speed != 0) {
		pos.x += sin(ugol) * speed;
		pos.y += cos(ugol) * speed;
	}

	if (key == 27) { // Если нажата клавиша ESC (код 27)
		exit(0); // Завершить программу
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Saper");

	glFrustum(-1, 1, -1, 1, 2, 80);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(handleKeyPress);

	glutMainLoop();

	return 0;
}