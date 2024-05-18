#pragma once

#include <glut.h>
#include <vector>

#include "Ball.h"

class Balls {

private:

	std::vector<Ball> m_Vec_Balls;

public:

	void setBall(float radius, Point3D center, Point3D color, Point3D velocity) {

		Ball temp_ball(radius, center, color, velocity);

		m_Vec_Balls.push_back(temp_ball);

	}

	void handleCollisionsWithBall() {

		for (size_t i = 0; i < m_Vec_Balls.size(); ++i) {
			for (size_t j = 0; j < m_Vec_Balls.size(); ++j) {
				if (i == j)
					continue;
				else {
					if (checkCollision(m_Vec_Balls[i], m_Vec_Balls[j])) {

						std::cout << "connection of balls" << std::endl;
						
						reflectVelocity(m_Vec_Balls[i], m_Vec_Balls[j]);

						//swapVelocity(m_Vec_Balls[i], m_Vec_Balls[j]);
					}
				}
			}
		}

	}

	void reflectVelocity(Ball& first, Ball& second) {
		// »спользуем закон сохранени€ импульса дл€ вычислени€ новых скоростей после столкновени€
		Point3D velocityDiff = { first.getVelocity().m_F_X - second.getVelocity().m_F_X, first.getVelocity().m_F_Y - second.getVelocity().m_F_Y, first.getVelocity().m_F_Z - second.getVelocity().m_F_Z };
		Point3D centerDiff = { first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X, first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y, first.getCoordsOfCenter().m_F_Z - second.getCoordsOfCenter().m_F_Z };

		float distance = sqrt(centerDiff.m_F_X * centerDiff.m_F_X + centerDiff.m_F_Y * centerDiff.m_F_Y + centerDiff.m_F_Z * centerDiff.m_F_Z);

		// Ќормализуем разность позиций
		centerDiff.normalize();

		// ¬ычисл€ем новые скорости
		float dotProduct = velocityDiff.dotProduct(centerDiff);
		Point3D reflection = velocityDiff - (centerDiff * dotProduct * 2.0);
		first.setVelocity(first.getVelocity() - reflection);
		second.setVelocity(second.getVelocity() + reflection);
	}


	bool checkCollision(Ball& first, Ball& second) {
		
		float dx = first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X;
		float dy = first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y;
		float dz = first.getCoordsOfCenter().m_F_Z - second.getCoordsOfCenter().m_F_Z;
		
		float distance = dx * dx + dy * dy + dz * dz;

		return distance <= pow(first.getRadius(), 2) + pow(second.getRadius(), 2) + 2 * first.getRadius() * second.getRadius();
	}

	void updateBalls(float deltaTime) {
		
		handleCollisionsWithBall();
		
		for (size_t count = 0; count < m_Vec_Balls.size(); ++count) {
			m_Vec_Balls[count].updatePos(deltaTime);
		}


		for (size_t count = 0; count < m_Vec_Balls.size(); ++count) {
			m_Vec_Balls[count].update();
		}
		
	}
};