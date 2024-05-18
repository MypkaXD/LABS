#pragma once

#include <glut.h>

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

	void update() {
		glColor3f(color.m_F_X, color.m_F_Y, color.m_F_Z); // red
		glPushMatrix();
		glTranslatef(center.m_F_X, center.m_F_Y, center.m_F_Z);
		glutSolidSphere(radius, 30, 30); // Рисуем сферу радиусом 1.0 с 30 долготами и 30 широтами
		glPopMatrix();
	}

	void updatePos(float deltaTime) {

		center.m_F_X += velocityX*deltaTime;
		center.m_F_Y += velocityY*deltaTime;
		center.m_F_Z += velocityZ*deltaTime;

		std::cout << center.m_F_Z << std::endl;	

		reflect(velocityZ, center.m_F_Z, radius, (center.m_F_Z < radius));
		reflect(velocityZ, center.m_F_Z, 5 - radius, (center.m_F_Z > 5 - radius));
		
		reflect(velocityX, center.m_F_X, radius, (center.m_F_X < radius));
		reflect(velocityX, center.m_F_X, 5 - radius, (center.m_F_X > 5 - radius));
		
		reflect(velocityY, center.m_F_Y, radius, (center.m_F_Y < radius));
		reflect(velocityY, center.m_F_Y, 5 - radius, (center.m_F_Y > 5 - radius));

		//velocityX -= 0.00002;
		//velocityY -= 0.00002;
		velocityZ -= g * deltaTime;

	}

	void reflect(float& speed, float& centerCoord, float wall, bool cond) {

		if (!cond)
			return;

		speed *= -0.85;
		centerCoord = wall;

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

