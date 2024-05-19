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

struct Color {
	float m_F_r = 0.0f;
	float m_F_g = 0.0f;
	float m_F_b = 0.0f;
	float m_F_alpha = 0.0f;

	Color(float r, float g, float b, float alpha) {
		this->m_F_r = r;
		this->m_F_g = g;
		this->m_F_b = b;
		this->m_F_alpha = alpha;
	}
};