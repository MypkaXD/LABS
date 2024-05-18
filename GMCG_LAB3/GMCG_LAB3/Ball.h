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

	Point3D color;

public:

	Ball(float radius, Point3D center, Point3D color, Point3D velocity) :
		radius(radius), center(center), color(color), velocityX(velocity.m_F_X), velocityY(velocity.m_F_Y), velocityZ(velocity.m_F_Z)
	{
	}

	void update(unsigned int& textureID) {
		glColor4f(color.m_F_X, color.m_F_Y, color.m_F_Z, 0.9); // red
		glPushMatrix();
		glTranslatef(center.m_F_X, center.m_F_Y, center.m_F_Z);
		
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, textureID);

		GLUquadric* quad = gluNewQuadric();
		gluQuadricTexture(quad, GL_TRUE); // Включение текстурирования на квадрике
		glColor4f(color.m_F_X, color.m_F_Y, color.m_F_Z, 0.4); // Зеленый цвет
		gluSphere(quad, radius, 32, 32);
		gluDeleteQuadric(quad);
		//gluNewQuadric() (radius, 30, 30); // Рисуем сферу радиусом 1.0 с 30 долготами и 30 широтами
		
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}

	void updatePos(float deltaTime) {

		//velocityZ -= g * deltaTime;

		center.m_F_X += velocityX * deltaTime;
		center.m_F_Y += velocityY * deltaTime;
		center.m_F_Z += velocityZ * deltaTime;

		collisionsWithWalls(velocityX, center.m_F_X, 4 - radius, center.m_F_X > 4 - radius);
		collisionsWithWalls(velocityX, center.m_F_X, -4 + radius, center.m_F_X < -4 + radius);

		collisionsWithWalls(velocityY, center.m_F_Y, 4 - radius, center.m_F_Y > 4 - radius);
		collisionsWithWalls(velocityY, center.m_F_Y, -4 + radius, center.m_F_Y < -4 + radius);

		collisionsWithWalls(velocityZ, center.m_F_Z, 4 - radius, center.m_F_Z > 4 - radius);
		collisionsWithWalls(velocityZ, center.m_F_Z, -4 + radius, center.m_F_Z < -4 + radius);

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

