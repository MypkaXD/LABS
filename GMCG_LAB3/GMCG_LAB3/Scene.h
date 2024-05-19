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

	float m_F_deltaTime = 0.001f;

	Balls m_Balls_balls;

	float m_F_x_border = 4;
	float m_F_y_border = 4;
	float m_F_z_border = 4;

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

	void update(const std::vector<GLuint> textures) {

		glPushMatrix();

		glScalef(2, 2, 2);

		createLight();

		m_Balls_balls.updateBalls(m_F_deltaTime, textures[1], m_F_x_border,m_F_y_border, m_F_z_border);

		draw_box(textures[2]);
		
		glPopMatrix();
	}

	void createLight() {

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);

		// Настройка освещения
		GLfloat light_diffuse[] = { 1, 1, 1 };
		GLfloat light_position[] = { 1.0, -1, -1, 0.0 };
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat light1_diffuse[] = { 1,0, 0};
		GLfloat light1_position[] = { 1, 1, 1.0, 1.0 };
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

		GLfloat light2_diffuse[] = { 0,1, 0 };
		GLfloat light2_position[] = { -1, -1, 1.0, 1.0 };
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	}

	void draw_box(GLuint texture) {

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		float ambient[4] = { 0.2, 0.2, 0.2,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

		// Начало рисования параллелепипеда
		glBegin(GL_QUADS);

		glColor4f(1, 1, 1, 0.2f);
		// Передняя грань (z = 1.0)
		//glColor3f(1, 0.0, 0.0); // Красный цвет
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, m_F_z_border);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(m_F_x_border, -m_F_y_border, m_F_z_border);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(m_F_x_border, m_F_y_border, m_F_z_border);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-m_F_x_border, m_F_y_border, m_F_z_border);

		// Задняя грань (z = -1.0)
		//glColor3f(0.0, 1, 0.0); // Зеленый цвет
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, -m_F_z_border);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(m_F_x_border, -m_F_y_border, -m_F_z_border);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(m_F_x_border, m_F_y_border, -m_F_z_border);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-m_F_x_border, m_F_y_border, -m_F_z_border);

		// Левая грань (x = -1.0)
		//glColor3f(0.0, 0.0, 1); // Синий цвет
		//glTexCoord2f(1.0f, 0.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, -m_F_z_border);
		//glTexCoord2f(1.0f, 1.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, m_F_z_border);
		//glTexCoord2f(0.0f, 1.0f);	glVertex3f(-m_F_x_border, m_F_y_border, m_F_z_border);
		//glTexCoord2f(0.0f, 0.0f);	glVertex3f(-m_F_x_border, m_F_y_border, -m_F_z_border);

		// Правая грань (x = 1.0)
		//glColor3f(1.0, 1.0, 0.0); // Желтый цвет
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(m_F_x_border, -m_F_y_border, -m_F_z_border);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(m_F_x_border, -m_F_y_border, m_F_z_border);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(m_F_x_border, m_F_y_border, m_F_z_border);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(m_F_x_border, m_F_y_border, -m_F_z_border);

		// Верхняя грань (y = 1.0)
		//glColor3f(1.0, 0.0, 1.0); // Фиолетовый цвет
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-m_F_x_border, m_F_y_border, -m_F_z_border);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(m_F_x_border, m_F_y_border, -m_F_z_border);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(m_F_x_border, m_F_y_border, m_F_z_border);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(-m_F_x_border, m_F_y_border, m_F_z_border);

		// Нижняя грань (y = -1.0)
		//glColor3f(0.0, 1.0, 1.0); // Голубой цвет
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, -m_F_z_border);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(m_F_x_border, -m_F_y_border, -m_F_z_border);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(m_F_x_border, -m_F_y_border, m_F_z_border);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-m_F_x_border, -m_F_y_border, m_F_z_border);

		glEnd();
		// Конец рисования параллелепипеда
		// Отключаем текстурирование

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

	}

	void init_scene() {
		createBalls();
	}

	int getRand(int leftBorder, int rightBorder) {
		return (leftBorder + rand() % (rightBorder - leftBorder + 1));
	}

	bool getCollisionsOfOtherBalls(int index, float x, float y, float z, float radius) {

		for (size_t count = 0; count < m_Balls_balls.getBalls().size(); ++count) {
			if (count == index)
				continue;
			else {
				float dx = x - m_Balls_balls.getBalls()[count].getCoordsOfCenter().m_F_X;
				float dy = y - m_Balls_balls.getBalls()[count].getCoordsOfCenter().m_F_Y;
				float dz = z - m_Balls_balls.getBalls()[count].getCoordsOfCenter().m_F_Z;

				float distance = sqrt(dx * dx + dy * dy + dz * dz);

				if (distance <= m_Balls_balls.getBalls()[count].getRadius() + radius)
					return false;
				else return true;
			}
		}

	}

	void createBall(float& radius, float& x, float& y, float& z, float& r, float& g, float& b, float& alpha, float& velocityX, float& velocityY, float& velocityZ, bool& isBig) {

		if (isBig) {
			radius = 1.0f;

			velocityX = 0.0f;
			velocityY = 0.0f;
			velocityZ = 0.0f;

			r = 1.0f;
			g = 0.0f;
			b = 0.0f;
			alpha = 1.0f;
		}
		else {
			radius = 0.5f;

			velocityX = 15;
			velocityY = 15;
			velocityZ = 15;
			
			r = 0.0f;
			g = 0.0f;
			b = 0.5f;
			alpha = 0.3f;
		}

		x = (float)getRand(-m_F_x_border/2, m_F_x_border/2);
		y = (float)getRand(-m_F_y_border/2, m_F_y_border/2);
		z = (float)getRand(-m_F_z_border/2, m_F_z_border/2);

	}

	void createBalls() {

		int countOfSmallBalls = 100;
		int countOfBigBalls = 1;

		float radius = 0;

		float x = 0;
		float y = 0;
		float z = 0;

		float r = 0;
		float g = 0;
		float b = 0;
		float alpha = 1;

		float velocityX = 0;
		float velocityY = 0;
		float velocityZ = 0;

		bool isBig = true;

		for (size_t count = 0; count < countOfBigBalls + countOfSmallBalls; ++count) {

			if (count == countOfBigBalls)
				isBig = false;

			do {
				createBall(radius, x, y, z, r, g, b, alpha, velocityX, velocityY, velocityZ, isBig);

			} while (getCollisionsOfOtherBalls(count, x, y, z, radius));

			Point3D coords(x, y, z);
			Color color(r, g, b, alpha);
			Point3D velocity(velocityX, velocityY, velocityZ);

			m_Balls_balls.setBall(radius, coords, color, velocity);
		}

		//float r = ((float)rand()) / RAND_MAX;
		//float g = ((float)rand()) / RAND_MAX;
		//float b = ((float)rand()) / RAND_MAX;

	}
};