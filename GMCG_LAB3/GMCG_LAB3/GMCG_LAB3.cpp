#include <iostream>
#include <vector>
#include <glut.h>

class Window {
private:

	// angle of rotation for the camera direction
	static float angle;
	// actual vector representing the camera's direction
	static float lx ,lz;
	// XZ position of the camera
	static float x, z;

public:

	Window(int argc, char** argv) {

		glutInit(&argc, argv);
		
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(400, 400);
		glutInitWindowPosition(100, 200);
		
		glutCreateWindow("Our first GLUT application!");
		
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(animation);
		glutSpecialFunc(processSpecialKeys);
		
		//init();
		
		glutMainLoop();
	}

	static void processSpecialKeys(int key, int xx, int yy) {

		std::cout << "HERE" << std::endl;

		float fraction = 0.1f;

		switch (key) {
		case GLUT_KEY_LEFT:
			angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT:
			angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
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

	static void animation() {

		// Clear Color and Depth Buffers

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset transformations
		glLoadIdentity();
		// Set the camera
		gluLookAt(x * 10, 1.0f, z * 10,
			10 * (x + lx), 1.0f, 10 * (z + lz),
			0.0f, 1.0f, 0.0f);

		// Draw ground
		glColor3f(0.9f, 0.9f, 0.9f);
		glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
		glEnd();

		// Draw 36 SnowMen
		for (int i = -3; i < 3; i++)
			for (int j = -3; j < 3; j++) {
				glPushMatrix();
				glTranslatef(i * 10.0, 0, j * 10.0);
				drawSnowMan();
				glPopMatrix();
			}

		glutSwapBuffers();

	}

	static void drawSnowMan() {

		glColor3f(1.0f, 1.0f, 1.0f);

		// Draw Body
		glTranslatef(0.0f, 0.75f, 0.0f);
		glutSolidSphere(0.75f, 20, 20);

		// Draw Head
		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f, 20, 20);

		// Draw Eyes
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f, 10, 10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f, 10, 10);
		glPopMatrix();

		// Draw Nose
		glColor3f(1.0f, 0.5f, 0.5f);
		glutSolidCone(0.08f, 0.5f, 10, 2);
	}

	static void draw_circle(GLdouble radius, GLint slices, GLint stacks, std::vector<GLfloat> color = {1.f, 1.f, 1.f} ) {

		glColor3f(color[0], color[1], color[2]);
		glutSolidSphere(radius, slices, stacks);

	}

	static void draw_triangle() {
		
		glBegin(GL_QUADS);
		
		glVertex3f(-2, -2, -5.0);
		glVertex3f(2, 0.0, -5.0);
		glVertex3f(0.0, 2, -5.0);
		glVertex3f(2.0, 2, -5.0);
		
		glEnd();
	}

	static void reshape(int width, int height) {

		if (height == 0)
			height = 1;
		
		float ratio = 1.0 * width / height;

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		glViewport(0, 0, width, height);

		gluPerspective(45, ratio, 1, 1000);

		glMatrixMode(GL_MODELVIEW);

	}

	static void display(void) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		draw_triangle();
		
		//glutSwapBuffers();
		
		//glFlush();
	}

	static void init() {
		glClearColor(0.4, 0.4, 0.6, 1.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-200.0, 200.0, -200.0, 200.0, -5.0, 5.0);
	}


};

float Window::angle = 0.0; // Инициализация статического поля класса

// actual vector representing the camera's direction
float Window::lx = 0.0f, Window::lz = -1.0f;
// XZ position of the camera
float Window::x = 0.0f, Window::z = 5.0f;

int main(int argc, char** argv) {

	Window window(argc, argv);

	return 0;
}