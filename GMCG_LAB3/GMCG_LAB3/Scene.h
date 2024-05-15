#include <glut.h>

#include "Types.h"

#pragma once

class Scene {

private:


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
	
	}

};