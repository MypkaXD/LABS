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
						

						//swapVelocity(m_Vec_Balls[i], m_Vec_Balls[j]);
					}
				}
			}
		}

	}


	bool checkCollision(Ball& first, Ball& second) {
		
		float dx = first.getCoordsOfCenter().m_F_X - second.getCoordsOfCenter().m_F_X;
		float dy = first.getCoordsOfCenter().m_F_Y - second.getCoordsOfCenter().m_F_Y;
		float dz = first.getCoordsOfCenter().m_F_Z - second.getCoordsOfCenter().m_F_Z;
		
		float distance = sqrt(dx * dx + dy * dy + dz * dz);

		return distance <= first.getRadius() + second.getRadius();
	}

	void updateBalls(float deltaTime) {
		
		handleCollisionsWithBall();
		
		for (size_t count = 0; count < m_Vec_Balls.size(); ++count) {
			m_Vec_Balls[count].updatePos(deltaTime);
			m_Vec_Balls[count].update();
		}

	}
};