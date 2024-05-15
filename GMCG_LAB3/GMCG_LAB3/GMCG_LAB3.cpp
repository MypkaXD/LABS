#include <iostream>
#include <tuple>
#include <vector>
#include <glut.h>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES

#include <math.h>

unsigned int texture;

float texCoord[] = { 0,1, 1,1, 0,1, 1,1,
					0, 0, 1,0, 0,0,1,0};
float vertex[] = { -1, -1, 0,	1, -1, 0,	 1, 1, 0,	-1, 1, 0,	 
					-1, -1, 2,		 1, -1, 2,		1,1,2,		-1, 1, 2 };

GLuint texInd[] = { 0, 1, 5,		5, 4 ,0,	1, 2, 6,	6, 5, 1,	2,3,7, 
7,6,2,		3,0,4,		4,7,3 };

int texIndCnt = sizeof(texInd) / sizeof(GLuint);

void draw_cube() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(6, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoord);
	
	glDrawElements(GL_TRIANGLES,texIndCnt, GL_UNSIGNED_INT, texInd);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

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

float angle_for_pyramid = 0;
float up_for_pyramid = 0;
float size_for_pyramid = 1;

void MoveCamera() {

	glRotatef(-xAlfa, 1, 0, 0);
	glRotatef(-zAlfa, 0, 0, 1);
	glTranslatef(-pos.x, -pos.y, -3);
}

void act_with_pyramids() {

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


	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(1, 0, 0);
	glScalef(size_for_pyramid, size_for_pyramid, size_for_pyramid);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glPopMatrix();

	static bool second_check = true;

	if (second_check)
		size_for_pyramid += 0.002;
	else
		size_for_pyramid -= 0.002;

	if (size_for_pyramid > 2)
		second_check = false;
	else if (size_for_pyramid < 0.2)
		second_check = true;

	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(1, 1, 0);
	glTranslatef(0, 6, up_for_pyramid);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	static bool check = true;

	if (check)
		up_for_pyramid += 0.001f;
	else
		up_for_pyramid -= 0.001f;

	if (up_for_pyramid > 4)
		check = false;
	else if (up_for_pyramid < 0)
		check = true;
	
	glPopMatrix();

	glPushMatrix();

	glVertexPointer(3, GL_FLOAT, 0, &pyramid);
	glColor3f(0, 1, 0);
	glTranslatef(0, 12, 0);
	glRotatef(angle_for_pyramid, 0, 0, 1);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	angle_for_pyramid += 0.1f;
	if (angle_for_pyramid >= 360.0f)
		angle_for_pyramid = 0;

	glColor3f(0, 0.5, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);


}

void draw_light_cube() {

	glPushMatrix(); // Сохраняем текущее состояние матрицы моделирования

	glTranslatef(6, 6, 1);


	glBegin(GL_QUADS);

	// Отрисовка граней куба
	glNormal3f(0, 0, 1);
	glColor3f(1, 1, 0);
	glVertex3f(-1, -1, 1);
	glColor3f(1, 0, 0);
	glVertex3f(1, -1, 1);
	glColor3f(0, 1, 0);
	glVertex3f(1, 1, 1);
	glColor3f(0, 0, 1);
	glVertex3f(-1, 1, 1);

	
	glNormal3f(0, 0, -1);
	glColor3f(0, 0, 1);
	glVertex3f(-1, -1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(1, -1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(1, 1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(-1, 1, -1);

	glNormal3f(0, 1, 0);
	glColor3f(0, 0, 1);
	glVertex3f(1, 1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(1, 1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(-1, 1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(-1, 1, 1);


	glColor3f(0, 0, 1);
	glNormal3f(0, -1, 0);
	glVertex3f(1, -1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(1, -1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(-1, -1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(-1, -1, 1);

	glNormal3f(1, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(1, -1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(1, 1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);

	glColor3f(0, 0, 1);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, -1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(-1, -1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(-1, 1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(-1, 1, 1);

	glEnd();

	glPopMatrix(); // Восстанавливаем предыдущее состояние матрицы моделирования
}



void display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	MoveCamera();
	act_with_pyramids();
	draw_light_cube();
	draw_cube();
	
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


void load_text(const char* filename, unsigned int *textureID) {

	int x, y, n;
	
	unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, n == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("LAB3");

	load_text("E://3labGCC//wall.png", &texture);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 80);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	//light
	
	
	// Включаем подсветку и материалы
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);    // Включаем источник света 0
	glEnable(GL_LIGHT1);    // Включаем источник света 1

	// Настройка источника света 0
	GLfloat light0_position[] = { -40.0, 40.0, 0}; // Положение источника света
	GLfloat light0_diffuse[] = { 0.5, 0.0, 0.0 };    // Цвет источника света
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

	// Настройка источника света 1
	GLfloat light1_position[] = { 40.0, -40.0, 1.0};
	GLfloat light1_diffuse[] = { 0.0, 1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(handleKeyPress);

	glutMainLoop();

	return 0;
}