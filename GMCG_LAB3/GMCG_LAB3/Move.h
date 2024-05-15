#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <glut.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Types.h"

class Moveming {
private:
    float m_F_Angle_Hor = 20;
    float m_F_Angle_Vert = 20;

    int m_I_center_x = 50; // значение по умолчанию, изменится при setWindowSize
    int m_I_center_y = 50; // значение по умолчанию, изменится при setWindowSize

    Point2D m_P2D_Pos = { 0, 0 };

public:
    void setWindowSize(int width, int height) {
        m_I_center_x = width / 2;
        m_I_center_y = height / 2;
    }

    void MoveCamera() {
        glRotatef(-m_F_Angle_Hor, 1, 0, 0);
        glRotatef(-m_F_Angle_Vert, 0, 0, 1);
        glTranslatef(-m_P2D_Pos.m_FX, -m_P2D_Pos.m_FY, -3);
    }

    void handleKeyPress(unsigned char key, int x, int y) {
        float angleRad = static_cast<float>(-m_F_Angle_Vert / 180.0 * M_PI);
        float speed = 0.0f;

        switch (key) {
        case 'w': speed = 0.1f; break;
        case 's': speed = -0.1f; break;
        case 'a':
            speed = 0.1f;
            angleRad -= M_PI * 0.5f;
            break;
        case 'd':
            speed = 0.1f;
            angleRad += M_PI * 0.5f;
            break;
        case 27: exit(0);
        }

        if (speed != 0) {
            m_P2D_Pos.m_FX += sin(angleRad) * speed;
            m_P2D_Pos.m_FY += cos(angleRad) * speed;
        }
    }

    void handleKeyMove(int x, int y) {

        int delta_x = x - m_I_center_x;
        int delta_y = y - m_I_center_y;

        m_F_Angle_Hor -= delta_y * 0.05f;
        m_F_Angle_Vert -= delta_x * 0.05f;

        if (std::abs(m_F_Angle_Hor) >= 360.f) m_F_Angle_Hor = 0.0f;
        if (std::abs(m_F_Angle_Vert) >= 360.f) m_F_Angle_Vert = 0.0f;

        glutWarpPointer(m_I_center_x, m_I_center_y);
        glutSetCursor(GLUT_CURSOR_NONE);
    }
};

#endif // MOVE_H