#ifndef GAME_H
#define GAME_H

#include <glut.h>

#include "Scene.h"
#include "Move.h"

class Game {
private:
    static int m_I_Width;
    static int m_I_Height;
    static Moveming move;

public:
    Game(int& argc, char** argv, int width, int height) {
        m_I_Width = width;
        m_I_Height = height;

        move.setWindowSize(m_I_Width, m_I_Height);

        glutInit(&argc, argv);
        set_settings();
        glutCreateWindow("LAB_3");

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1, 1, -1, 1, 2, 80);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);

        set_callback();
        glutMainLoop();
    }

    void set_settings() {
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(m_I_Width, m_I_Height);
        glutInitWindowPosition(100, 100);
    }

    static void set_callback() {
        glutDisplayFunc(display);
        glutKeyboardFunc(handleKeyPress);
        glutIdleFunc(display);
        glutPassiveMotionFunc(handleKeyMove);
    }

    static void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        move.MoveCamera();
        Scene sc;
        sc.draw_floor();
        glPopMatrix();
        glutSwapBuffers();
    }

    static void handleKeyPress(unsigned char key, int x, int y) {
        move.handleKeyPress(key, x, y);
    }

    static void handleKeyMove(int x, int y) {
        move.handleKeyMove(x, y);
    }

    static int getWindowWidth() { return m_I_Width; }
    static int getWindowHeight() { return m_I_Height; }
};

int Game::m_I_Width = 100;
int Game::m_I_Height = 100;
Moveming Game::move;

#endif // GAME_H
