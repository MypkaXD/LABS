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
    static Scene sc;

    //static GLuint texture;

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
        glFrustum(width / -2, width / 2, height / -2, height / 2, 2, 80);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);

        //loadTexture();
        //glEnable(GL_TEXTURE_2D);

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_ONE, GL_ONE);

        //glEnable(GL_ALPHA_TEST);
        //glAlphaFunc(GL_LESS, 0.4);

        set_callback();
        glutMainLoop();
    }
    /*
    void loadTexture() {

        // «агрузка текстуры с помощью stb_image
        int width, height, nrChannels;
        unsigned char* data = stbi_load("path_to_texture_image.jpg", &width, &height, &nrChannels, 0);
        if (data) {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            // Ќастройка параметров текстуры
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // «агрузка текстуры в OpenGL
            if (nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }

            stbi_image_free(data);
        }
        else {
            printf("Failed to load texture\n");
        }

    }
    */

    void set_settings() {
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(m_I_Width, m_I_Height);
        glutInitWindowPosition(0, 0);
    }

    static void set_callback() {
        glutDisplayFunc(display);
        glutKeyboardFunc(handleKeyPress);
        glutIdleFunc(idle);
        glutPassiveMotionFunc(handleKeyMove);
        glutReshapeFunc(resize);
    }

    static void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        move.MoveCamera();
        sc.init_scene();
        glPopMatrix();
        glutSwapBuffers();
    }

    static void idle() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0.25, 0);
        glPushMatrix();
        move.MoveCamera();
        sc.update();
        glPopMatrix();
        glutSwapBuffers();
    }

    static void handleKeyPress(unsigned char key, int x, int y) {
        move.handleKeyPress(key, x, y);
    }

    static void handleKeyMove(int x, int y) {
        move.handleKeyMove(x, y);
    }

    static void resize(int width, int height)
    {
        const float ar = (float)width / (float)height; // ¬ычисление аспектного соотношени€
        /*
            „то это такое? јспектное соотношение (aspect ratio, ar) Ч это отношение ширины 
                окна к его высоте.
            ѕочему это важно? јспектное соотношение помогает нам сохранить 
                правильные пропорции изображени€ при изменении размера окна. 
                Ќапример, квадрат должен оставатьс€ квадратом, а не превращатьс€ в
                пр€моугольник, когда окно измен€етс€.
        */


        glViewport(0, 0, width, height); // ”становка области просмотра
        /*
            „то это делает? Ёта функци€ устанавливает размер и положение области 
                просмотра в окне. ¬ данном случае область просмотра устанавливаетс€ так, 
                чтобы занимать все окно от (0,0) до (width, height).
        */

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        /*
            glMatrixMode(GL_PROJECTION); переключает режим работы с матрицами на 
                проекционную матрицу. Ёто означает, что последующие команды будут измен€ть 
                матрицу проекции.
            glLoadIdentity(); сбрасывает текущую матрицу (в данном случае проекционную)
                на единичную матрицу, котора€ не оказывает никакого вли€ни€ на координаты.
        */

        // »спользуем корректные параметры дл€ frustum с учетом аспекта
        float frustumHeight = 2.0f;
        float frustumWidth = frustumHeight * ar;

        glFrustum(-frustumWidth / 2, frustumWidth / 2, -frustumHeight / 2, frustumHeight / 2, 2.0, 500.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    static int getWindowWidth() { return m_I_Width; }
    static int getWindowHeight() { return m_I_Height; }
};

int Game::m_I_Width = 100;
int Game::m_I_Height = 100;
Moveming Game::move;
Scene Game::sc;
//GLuint Game::texture;

#endif // GAME_H
