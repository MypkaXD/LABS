#pragma once

#include <glut.h>
#include <vector>

#include "Ball.h"

class Balls {

private:

	std::vector<Ball> m_Vec_Balls;

	unsigned int textureID;

public:

	void setBall(float radius, Point3D center, Color color, Point3D velocity) {

		Ball temp_ball(radius, center, color, velocity);

		m_Vec_Balls.push_back(temp_ball);

	}

	std::vector<Ball>& getBalls() {
		return m_Vec_Balls;
	}

	void handleCollisionsWithBall() {

		for (size_t i = 0; i < m_Vec_Balls.size(); ++i) {
			for (size_t j = i + 1; j < m_Vec_Balls.size(); ++j) {
				if (checkCollision(m_Vec_Balls[i], m_Vec_Balls[j])) {

					//std::cout << "connection of balls" << std::endl;

					collisionsWithBalls(m_Vec_Balls[i], m_Vec_Balls[j]);

					//swapVelocity(m_Vec_Balls[i], m_Vec_Balls[j]);
				}
			}
		}

	}

	void collisionsWithBalls(Ball& first, Ball& second) {

		float sumOfRadius = first.getRadius() + second.getRadius();

		float dx = first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X;
		float dy = first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y;
		float dz = first.getCoordsOfCenter().m_F_Z - second.getCoordsOfCenter().m_F_Z;

		float distance = sqrt(dx * dx + dy * dy + dz * dz);

		float overlapX = dx * (sumOfRadius - distance) / distance;
		float overlapY = dy * (sumOfRadius - distance) / distance;
		float overlapZ = dz * (sumOfRadius - distance) / distance;

		float im1 = 1 / (first.getRadius() * first.getRadius() * first.getRadius());
		float im2 = 1 / (second.getRadius() * second.getRadius() * second.getRadius());

		Point3D moveFirstBall(first.getCoordsOfCenter().m_F_X + overlapX * (im1) / (im1 + im2),
			first.getCoordsOfCenter().m_F_Y + overlapY * (im1) / (im1 + im2),
			first.getCoordsOfCenter().m_F_Z + overlapZ * (im1) / (im1 + im2));

		Point3D moveSecondBall(second.getCoordsOfCenter().m_F_X - overlapX * (im2) / (im1 + im2),
			second.getCoordsOfCenter().m_F_Y - overlapY * (im2) / (im1 + im2),
			second.getCoordsOfCenter().m_F_Z - overlapZ * (im2) / (im1 + im2));

		first.setCenter(moveFirstBall);
		second.setCenter(moveSecondBall);

		float velocityDx = first.getVelocity().m_F_X - second.getVelocity().m_F_X;
		float velocityDy = first.getVelocity().m_F_Y - second.getVelocity().m_F_Y;
		float velocityDz = first.getVelocity().m_F_Z - second.getVelocity().m_F_Z;
		
		float lenghtOfOverlap = sqrt(overlapX * overlapX + overlapY * overlapY + overlapZ * overlapZ);

		float velocityNormal = velocityDx * (overlapX / lenghtOfOverlap) +
			velocityDy * (overlapY / lenghtOfOverlap) +
			velocityDz * (overlapZ / lenghtOfOverlap);

		if (velocityNormal > 0)
			return;

		float m1 = (float)1 / im1;
		float m2 = (float)1 / im2;

		float th = atan((first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y) / 
			(first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X));
		float cth = cosf(th);
		float sth = sinf(th);

		float v1t = (cth * ((m1 - m2) * first.getVelocity().m_F_X + 2 * m2 * second.getVelocity().m_F_X) + sth *
			((m1 - m2) * first.getVelocity().m_F_Y + 2 * m2 * second.getVelocity().m_F_Y)) / (m1 + m2);
		float v2t = v1t + (cth * (first.getVelocity().m_F_X - second.getVelocity().m_F_X) +
			sth * (first.getVelocity().m_F_Y - second.getVelocity().m_F_Y));
		
		float v1n = (first.getVelocity().m_F_Y * cth) - (sth * first.getVelocity().m_F_X);
		float v2n = (cth * second.getVelocity().m_F_Y - sth * second.getVelocity().m_F_X);

		Point3D newVelocityForFirst((v1t * cth) - (v1n * sth), (v1n * cth) + (v1t * sth), first.getVelocity().m_F_Z);
		Point3D newVelocityForSecond((v2t * cth) - (v2n * sth), (v2n * cth) + (v2t * sth), second.getVelocity().m_F_Z);

		first.setVelocity(newVelocityForFirst);
		second.setVelocity(newVelocityForSecond);

	}

	bool checkCollision(Ball& first, Ball& second) {
		
		float dx = first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X;
		float dy = first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y;
		float dz = first.getCoordsOfCenter().m_F_Z - second.getCoordsOfCenter().m_F_Z;
		
		float distance = sqrt(dx * dx + dy * dy + dz * dz);

		return distance < first.getRadius() + second.getRadius();
	}

	void updateBalls(float deltaTime, const GLuint& texture, float& m_F_x_border, float& m_F_y_border, float& m_F_z_border) {

		handleCollisionsWithBall();
		
		for (size_t count = 0; count < m_Vec_Balls.size(); ++count) {
			m_Vec_Balls[count].updatePos(deltaTime, m_F_x_border, m_F_y_border, m_F_z_border);
			m_Vec_Balls[count].update(texture);
		}

	}
};