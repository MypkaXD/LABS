#include <stdlib.h>
#include <glut.h>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#pragma once

class Game {

private:

	static GLuint VBO; // Объявление переменной VBO;

	// angle of rotation for the camera direction
	static float angle, yAngle;
	// actual vector representing the camera's direction
	static float lx, ly, lz;
	// XZ position of the camera
	static float x , z ;
	static float roll ;

public:

	Game(int argc, char** argv, const int& width, const int& height) {

		glutInit(&argc, argv); // инициализация системы GLUT перед использованием других функций GLUT

		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // устанавливает режим отображения для окна GLUT
		
		/*
			GLUT_RGB указывает, что окно будет использовать RGB цветовую модель (красный, зеленый, синий) для отображения.
			GLUT_DOUBLE указывает, что будет использоваться двойная буферизация. Это означает, что в окне будет использоваться два буфера: передний (отображаемый пользователю) и задний (в который происходит рисование). После завершения рисования на заднем буфере, он будет автоматически сменен с передним буфером, что позволяет избежать эффекта мерцания.
			GLUT_DEPTH указывает, что будет использоваться буфер глубины (depth buffer) для реализации эффекта глубины (задает порядок, в котором детали будут
		*/

		glutInitWindowSize(width, height); // устанавливает начальный размер окна GLUT.
		glutInitWindowPosition(150, 150); // устанавливает начальное положение окна GLUT на экране
		
		glutCreateWindow("Our first GLUT application!"); // создает окно GLUT с указанным заголовком

		initializeGlutCallbacks();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);

		glutMainLoop(); // основной цикл программы
	}

	static void display();

	static void changeSize(int w, int h);

	static void initializeGlutCallbacks();

	static void processNormalKeys(unsigned char key, int xx, int yy) {
		float fraction = 0.1f;
		if (key == 'w') {
			x += lx * fraction;
			z += lz * fraction;
		}
		else if (key == 'a') {
			x += sin(angle - M_PI / 2.0) * fraction;
			z += -cos(angle - M_PI / 2.0) * fraction;
		}
		else if (key == 's') {
			x -= lx * fraction;
			z -= lz * fraction;
		}
		else if (key == 'd') {
			x += sin(M_PI / 2.0 + angle) * fraction;
			z += -cos(M_PI / 2.0 + angle) * fraction;
		}
		else if (key == 'x') {
			roll += 0.5f;
		}
		else if (key == 'z') {
			roll -= 0.5f;
		}

		if (key == 27)
			exit(0);
	}

	static void processSpecialKeys(int key, int xx, int yy) {

		float fraction = 0.1f;

		switch (key) {
		case GLUT_KEY_LEFT:
			x += sin(angle - M_PI / 2.0) * fraction;
			z += -cos(angle - M_PI / 2.0) * fraction;
			break;
		case GLUT_KEY_RIGHT:
			x += sin(M_PI / 2.0 + angle) * fraction;
			z += -cos(M_PI / 2.0 + angle) * fraction;
			break;
		case GLUT_KEY_UP:
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * fraction;
			z -= lz * fraction;
			break;
		}
	}
};