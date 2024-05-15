#include <glut.h>
#include <vector>

#pragma once


class Saper {

private:

	static int fields_H;
	static int fields_W;

	static int windowWidth;
	static int windowHeight;

	static int count_of_mines;
	static int count_of_open_mines;
	
	static struct Field {
		bool isMine = false;
		bool isFlag = false;
		bool isOpen = false;

		int count_of_mines_around = 0;
	};

	static std::vector<std::vector<Field>> map;

	static bool isFail;


public:

	Saper(int& argc, char**& argv) {

		init_mines();

		init(argc, argv);

	}

	static bool isMineInMap(int x, int y) {
		return (x >= 0) && (y >= 0) && (x < fields_W) && (y < fields_H);
	}

	static void init_mines() {

		map.clear();

		map.resize(fields_W);
		for (size_t count = 0; count < map.size(); ++count)
			map[count].resize(fields_H);

		srand(time(0));

		count_of_open_mines = 0;
		count_of_mines = 10;

		for (size_t count = 0; count < count_of_mines; ++count) {

			int x = rand() % fields_W;
			int y = rand() % fields_H;

			if (map[x][y].isMine)
				--count;
			else {
				map[x][y].isMine = true;

				for (int dx = -1; dx < 2; ++dx) {
					for (int dy = -1; dy < 2; ++dy) {
						if (isMineInMap(x + dx, y + dy))
							map[x + dx][y + dy].count_of_mines_around += 1;
					}
				}
				
			}

		}

	}

	static void Line(float x1, float y1, float x2, float y2) {
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}

	static void draw_count(int a) {

		glLineWidth(3);
		glColor3f(1.0f, 0, 0);
		glBegin(GL_LINES);
			if ((a != 1) && (a != 4)) Line(0.3, 0.85, 0.7, 0.85);
			if ((a != 0) && (a != 1) && (a != 7)) Line(0.3, 0.5, 0.7, 0.5);
			if ((a != 1) && (a != 4) && (a != 7)) Line(0.3, 0.15, 0.7, 0.15);

			if ((a != 5) && (a != 6)) Line(0.7, 0.5, 0.7, 0.85);
			if ((a != 2)) Line(0.7, 0.5, 0.7, 0.15);

			if ((a != 1) && (a != 2) && (a != 3)) Line(0.3, 0.5, 0.3, 0.85);
			if ((a == 0) || (a == 2) || (a==6) || (a == 8)) Line(0.3, 0.5, 0.3, 0.15);
		glEnd();
	}

	static void draw_mine(const int& i, const int& j) {

		if (map[i][j].isMine) {

			glBegin(GL_QUADS);

			glColor3f(1.f, 1.f, 1.f);

			glVertex2f(0, 0);
			glVertex2f(1, 0);
			glVertex2f(1, 1);
			glVertex2f(0, 1);

			glColor3f(0.5f, 0.5f, 0.5f);

			glVertex2f(0, 0);
			glVertex2f(0.98, 0);
			glVertex2f(0.98, 0.98);
			glVertex2f(0, 0.98);

			glEnd();
			
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);  // Красный цвет для вырезанной части
			glVertex2f(0.3f, 0.3f);     // Левая нижняя вершина прямоугольника
			glVertex2f(0.7f, 0.3f);      // Правая нижняя вершина прямоугольника
			glVertex2f(0.5f, 0.7f);       // Верхняя вершина прямоугольника
			glEnd();


		}
		else {
			glBegin(GL_QUADS);

			glColor3f(1.f, 1.f, 1.f);

			glVertex2f(0, 0);
			glVertex2f(1, 0);
			glVertex2f(1, 1);
			glVertex2f(0, 1);

			glColor3f(0.5f, 0.5f, 0.5f);

			glVertex2f(0, 0);
			glVertex2f(0.98, 0);
			glVertex2f(0.98, 0.98);
			glVertex2f(0, 0.98);

			glEnd();
		}

	}

	static void draw_flag() {

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.7f, 0.7f);
		glVertex2f(0.5f, 0.9f);
		glEnd();
		glLineWidth(3);
		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(0.5f, 0.2f);
		glVertex2f(0.5f, 0.9f);
		glEnd();

	}

	static void draw_field() {

		float cellWidth = static_cast<float>(windowWidth) / fields_W;
		float cellHeight = static_cast<float>(windowHeight) / fields_H;

		glTranslatef(-1, -1, 0);
		glScalef(2.0 / fields_W, 2.0 / fields_H, 1);

		//std::cout << "fields_H: " << fields_H << std::endl;
		//std::cout << "fields_W: " << fields_W << std::endl;

		//std::cout << "cellWidth: " << cellWidth << std::endl;
		//std::cout << "cellHeight: " << cellHeight << std::endl;

		for (size_t j = 0; j < fields_H; ++j) {
			for (size_t i = 0; i < fields_W; ++i) {
				glPushMatrix();
				//std::cout << "X: " << i * cellWidth << "\t Y: " << j * cellHeight << std::endl;
				glTranslatef(i, j, 0);
				if (map[i][j].isOpen) {
					draw_mine(i, j);
					if (map[i][j].count_of_mines_around > 0) {
						if (!map[i][j].isMine)
							draw_count(map[i][j].count_of_mines_around);
					}
				}
				else {
					glBegin(GL_QUADS);

					glColor3f(1.f, 1.f, 1.f);

					glVertex2f(0, 0);
					glVertex2f(1, 0);
					glVertex2f(1, 1);
					glVertex2f(0, 1);

					glColor3f(0.5f, 1 , 0.5f);

					glVertex2f(0, 0);
					glVertex2f(0.98, 0);
					glVertex2f(0.98, 0.98);
					glVertex2f(0, 0.98);

					glEnd();
				
					if (map[i][j].isFlag)
						draw_flag();
				}
				
				glPopMatrix();


			}
		}

	}

	static void display() {

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();

		draw_field();

		glutSwapBuffers();
	}


	static void init(int& argc, char**& argv) {

		glutInit(&argc, argv);;

		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
		glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(100, 100);

		glutCreateWindow("Saper");

		glutDisplayFunc(display);
		glutIdleFunc(display);
		glutMouseFunc(mouse_click);
		glutReshapeFunc(reshape);

		glutMainLoop();
	}

	static void mouse_click(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (isFail) {
				restart();
				return;
			}

			std::cout << "Left mouse button clicked at (" << x << ", " << windowHeight - y << ")" << std::endl;
			
			float cellWidth = static_cast<float>(windowWidth) / fields_W;
			float cellHeight = static_cast<float>(windowHeight) / fields_H;

			for (size_t i = 0; i < fields_W; ++i) {
				for (size_t j = 0; j < fields_H; ++j) {
					if (	(x >= i * cellWidth) && (x <= (i+1)* cellWidth)
						&& (windowHeight - y >= (cellHeight * j)) && (windowHeight - y <= (cellHeight * (j + 1)))
						&& !map[i][j].isFlag) {
						if (map[i][j].isMine) {
							open_all();
							isFail = true;
							return;
						}
						else {
							++count_of_open_mines;
							if (count_of_open_mines < count_of_mines) {
								open_empty(i, j);
								return;
							}
							else
							{
								isFail = true;
								return;
							}
						}
					}
				}
			}

		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

			float cellWidth = static_cast<float>(windowWidth) / fields_W;
			float cellHeight = static_cast<float>(windowHeight) / fields_H;

			for (size_t i = 0; i < fields_W; ++i) {
				for (size_t j = 0; j < fields_H; ++j) {
					if ((x >= i * cellWidth) && (x <= (i + 1) * cellWidth)
						&& (windowHeight - y >= (cellHeight * j)) && (windowHeight - y <= (cellHeight * (j + 1)))) {
						if (map[i][j].isFlag)
							map[i][j].isFlag = false;
						else {

							map[i][j].isFlag = true;
						}
						return;
					}
				}
			}

		}
	}

	static void open_empty(int i, int j) {
		if (!isMineInMap(i, j) || map[i][j].isOpen)
			return; 

		map[i][j].isOpen = true;

		if (map[i][j].count_of_mines_around == 0) {
			open_empty(i + 1, j + 1);
			open_empty(i + 1, j);
			open_empty(i + 1, j - 1);
			open_empty(i, j + 1);
			open_empty(i, j - 1);
			open_empty(i - 1, j + 1);
			open_empty(i - 1, j);
			open_empty(i - 1, j - 1);
		}
	}

	static void reshape(int w, int h) {
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		windowWidth = w;
		windowHeight = h;
	}

	static void open_all() {
		for (size_t i = 0; i < map.size(); ++i) {
			for (size_t j = 0; j < map[i].size(); ++j) {
				map[i][j].isOpen = true;
			}
		}
	}

	static void restart() {

		isFail = false;

		init_mines();
	}
};


int Saper::windowWidth = 1000;
int Saper::windowHeight = 1000;

int Saper::fields_H = 10;
int Saper::fields_W = 10;

int Saper::count_of_open_mines = 0;
int Saper::count_of_mines = 10;

bool Saper::isFail = false;

std::vector<std::vector<Saper::Field>> Saper::map;