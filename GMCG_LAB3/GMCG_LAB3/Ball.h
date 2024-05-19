#pragma once

#include <glut.h>

#include "Game.h"
#include "Types.h"

class Ball {

private:

	float radius;
	
	Point3D center;

	float g = 9.8;

	float velocityX = 0;
	float velocityY = 0;
	float velocityZ = 0;

	float koef = 0.7;

	float angle = 30.0f;

	Color color;

public:

	Ball(float radius, Point3D center, Color color, Point3D velocity) :
		radius(radius), center(center), color(color), velocityX(velocity.m_F_X), velocityY(velocity.m_F_Y), velocityZ(velocity.m_F_Z)
	{
	}

	void update(const GLuint& texture) {

		glPushMatrix();
		glTranslatef(center.m_F_X, center.m_F_Y, center.m_F_Z);
		glRotatef(angle, 0, 1, 0);

		angle += 0.5f;
		if (angle >= 360.0f)
			angle = 0.0f;

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLUquadric* quad = gluNewQuadric();
		gluQuadricTexture(quad, GL_TRUE); // Включение текстурирования на квадрике
		glColor4f(color.m_F_r, color.m_F_g, color.m_F_b, color.m_F_alpha); // Зеленый цвет
		gluSphere(quad, radius, 32, 32);
		gluDeleteQuadric(quad);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}

	void updatePos(float deltaTime, float& m_F_x_border, float& m_F_y_border, float& m_F_z_border) {

		//velocityZ -= g * deltaTime;

		center.m_F_X += velocityX * deltaTime;
		center.m_F_Y += velocityY * deltaTime;
		center.m_F_Z += velocityZ * deltaTime;

		collisionsWithWalls(velocityX, center.m_F_X, m_F_x_border - radius, center.m_F_X > m_F_x_border - radius);
		collisionsWithWalls(velocityX, center.m_F_X, -m_F_x_border + radius, center.m_F_X < -m_F_x_border + radius);

		collisionsWithWalls(velocityY, center.m_F_Y, m_F_y_border - radius, center.m_F_Y > m_F_y_border - radius);
		collisionsWithWalls(velocityY, center.m_F_Y, -m_F_y_border + radius, center.m_F_Y < -m_F_y_border + radius);

		collisionsWithWalls(velocityZ, center.m_F_Z, m_F_z_border - radius, center.m_F_Z > m_F_z_border - radius);
		collisionsWithWalls(velocityZ, center.m_F_Z, -m_F_z_border + radius, center.m_F_Z < -m_F_z_border + radius);

	}

	void collisionsWithWalls(float& velocity, float& center, float wall, bool condition) {

		if (!condition)
			return;
		
		velocity *= -1; // -0.85
		center = wall;

	}

	Point3D& getCoordsOfCenter() {
		return center;
	}

	float& getRadius() {
		return radius;
	}

	Point3D getVelocity() {
		return { velocityX, velocityY, velocityZ };
	}

	void setCenter(Point3D& newCenter) {

		center = newCenter;

	}

	void setVelocity(Point3D vel) {
		
		velocityX = vel.m_F_X;
		velocityY = vel.m_F_Y;
		velocityZ = vel.m_F_Z;

	}
};

