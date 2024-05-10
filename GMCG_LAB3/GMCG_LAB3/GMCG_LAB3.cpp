#include <stdint.h>
#include <iostream>
#include <glut.h>

void display();
void reshape(int w, int h);
void timer(int);

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0,0);
	glutInitWindowSize(1080, 720);

	glutCreateWindow("Window 1");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000, timer, 0);
	init();

	glutMainLoop();

	return 0;
}

float x_position = -10.0f;
float angle = 0.0f;
bool state = true;

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	//draw

	glShadeModel(GL_SMOOTH); // будет смешивание цветов (По умолчанию)
	//glShadeModel(GL_FLAT); // НЕ будет смешивание цветов

	glTranslatef(0.0f, 0.0f, -8.0f);
	glRotatef(angle, 1.0f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	//glPointSize(10.0f);
	// указываем вершины против часовой стрелки
	glBegin(GL_QUADS);

	//front
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	//back
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	//right
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);
	//left
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	//top
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	//bottom
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, 1, 2.0, 50.0);

	glMatrixMode(GL_MODELVIEW);

}

void timer(int) {

	glutPostRedisplay();
	glutTimerFunc(1000 / 144, timer, 0);

	angle += 0.8f;

	if (angle > 360.0f) {
		angle = 360.0f - angle;
	}

	/*
	switch (state)
	{
	case true:
		if (x_position < -5.0f)
			x_position += 0.15f;
		else
			state = false;
		break;
	case false:
		if (x_position > -15.0f)
			x_position -= 0.15f;
		else
			state = true;
		break;
	default:
		break;
	}
	*/
	

}