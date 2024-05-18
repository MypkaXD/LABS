#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glut.h>
#include <vector>

#include "Types.h"
#include "Balls.h"
#include "Game.h"

#pragma once

class Scene {

private:

	float deltaTime = 0.001f;

	Balls balls;

	float x_border = 4;
	float y_border = 4;
	float z_border = 4;

public:

	void draw_floor() {
		
		Point3D vertices[] = {
		{1,1,0},
		{1,-1,0},
		{-1,-1,0},
		{-1,1,0}
		};

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

		glDisableClientState(GL_VERTEX_ARRAY);
	
	}

	void update() {

		glPushMatrix();

		glScalef(2, 2, 2);

		balls.updateBalls(deltaTime);


		//draw_floor();

		draw_box();
		
		glPopMatrix();
	}

	void draw_box() {

		// Включаем текстурирование
		glEnable(GL_TEXTURE_2D);

		// Загружаем текстуру с помощью stb_image
		int width, height, channels;
		unsigned char* textureData = stbi_load("C:\\dev\\Source\\LABS\\GMCG_LAB3\\wall.png", &width, &height, &channels, 0);
		
		unsigned int textureID;
		
		if (textureData) {

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			
			glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA: GL_RGB , width, height, 0, channels == 4 
				? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);
			
			stbi_image_free(textureData);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			printf("Failed to load texture\n");
			return;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);

		// Начало рисования параллелепипеда
		glBegin(GL_QUADS);

		glColor3f(1, 1, 1);
		// Передняя грань (z = 1.0)
		//glColor3f(1, 0.0, 0.0); // Красный цвет
		glVertex3f(-x_border, -y_border, z_border);
		glVertex3f(x_border, -y_border, z_border);
		glVertex3f(x_border,y_border, z_border);
		glVertex3f(-x_border, y_border, z_border);

		// Задняя грань (z = -1.0)
		//glColor3f(0.0, 1, 0.0); // Зеленый цвет
		glVertex3f(-x_border, -y_border, -z_border);
		glVertex3f(x_border, -y_border, -z_border);
		glVertex3f(x_border, y_border, -z_border);
		glVertex3f(-x_border, y_border, -z_border);

		// Левая грань (x = -1.0)
		//glColor3f(0.0, 0.0, 1); // Синий цвет
		glVertex3f(-x_border, -y_border, -z_border);
		glVertex3f(-x_border, -y_border, z_border);
		glVertex3f(-x_border, y_border, z_border);
		glVertex3f(-x_border, y_border, -z_border);

		// Правая грань (x = 1.0)
		//glColor3f(1.0, 1.0, 0.0); // Желтый цвет
		glVertex3f(x_border, -y_border, -z_border);
		glVertex3f(x_border, -y_border, z_border);
		glVertex3f(x_border, y_border, z_border);
		glVertex3f(x_border, y_border, -z_border);

		// Верхняя грань (y = 1.0)
		//glColor3f(1.0, 0.0, 1.0); // Фиолетовый цвет
		glVertex3f(-x_border, y_border, -z_border);
		glVertex3f(x_border, y_border, -z_border);
		glVertex3f(x_border, y_border, z_border);
		glVertex3f(-x_border, y_border, z_border);

		// Нижняя грань (y = -1.0)
		//glColor3f(0.0, 1.0, 1.0); // Голубой цвет
		glVertex3f(-x_border, -y_border, -z_border);
		glVertex3f(x_border, -y_border, -z_border);
		glVertex3f(x_border, -y_border, z_border);
		glVertex3f(-x_border, -y_border, z_border);

		glEnd();
		// Конец рисования параллелепипеда
		// Отключаем текстурирование
		glDisable(GL_TEXTURE_2D);
	}

	void init_scene() {

		//balls.setBall(1, { 0,0,20 }, { 1,0,0 }, { 5,5,5 });
		//balls.setBall(1, { 0,0,10 }, { 0,1,0 }, { 3,4,7 });

		createBall();
		balls.loadText();

		//draw_floor();
	}

	int getRand(int leftBorder, int rightBorder) {

		return (leftBorder + rand() % (rightBorder - leftBorder + 1));
	}

	bool getCollisionsOfOtherBalls(int index, float x, float y, float z, float radius) {

		for (size_t count = 0; count < balls.getBalls().size(); ++count) {
			if (count == index)
				continue;
			else {
				float dx = x - balls.getBalls()[count].getCoordsOfCenter().m_F_X;
				float dy = y - balls.getBalls()[count].getCoordsOfCenter().m_F_Y;
				float dz = z - balls.getBalls()[count].getCoordsOfCenter().m_F_Z;

				float distance = sqrt(dx * dx + dy * dy + dz * dz);

				if (distance <= balls.getBalls()[count].getRadius() + radius)
					return false;
				else return true;
			}
		}

	}

	void createBall() {

		int countOfBalls = 100;

		float radius = 0;

		float x = 0;
		float y = 0;
		float z = 0;

		float r = 0;
		float g = 0;
		float b = 0;

		float velocityX = 0;
		float velocityY = 0;
		float velocityZ = 0;

		bool isBig = true;

		for (size_t count = 0; count < countOfBalls; ++count) {
			
			do {

				if (isBig){
					radius = 1;
				}
				else
					radius = 0.5;

				x = (float)getRand(-4, 4);
				y = (float)getRand(-4, 4);
				z = (float)getRand(-4, 4);

			} while (getCollisionsOfOtherBalls(count, x, y, z, radius));

			if (isBig) {
				velocityX = 0;
				velocityY = 0;
				velocityZ = 0;
			}
			else {
				velocityX = 15;
				velocityY = 15;
				velocityZ = 15;
			}

			//float r = ((float)rand()) / RAND_MAX;
			//float g = ((float)rand()) / RAND_MAX;
			//float b = ((float)rand()) / RAND_MAX;
		
			if (isBig) {
				r = 1;
				g = 0;
				b = 0;

				isBig = false;
			}
			else {
				r = 0;
				g = 0;
				b = 0.5;
			}

			Point3D coords(x, y, z);
			Point3D color(r, g, b);
			Point3D velocity(velocityX, velocityY, velocityZ);

			balls.setBall(radius, coords, color, velocity);
		}

	}
};