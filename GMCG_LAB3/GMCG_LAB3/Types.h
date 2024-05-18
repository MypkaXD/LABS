#pragma once

struct Point2D {
	float m_FX = 0.0f;
	float m_FY = 0.0f;
};

struct Point3D {
	float m_F_X = 0.0f;
	float m_F_Y = 0.0f;
	float m_F_Z = 0.0f;

	Point3D(float x, float y, float z) {
		this->m_F_X = x;
		this->m_F_Y = y;
		this->m_F_Z = z;
	}
};