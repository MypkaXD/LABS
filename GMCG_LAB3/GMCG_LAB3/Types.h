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

	Point3D operator*(const float scalar) const {
		return { m_F_X * scalar, m_F_Y * scalar, m_F_Z * scalar };
	}

	Point3D operator-(const Point3D scalar) const {
		return { m_F_X - scalar.m_F_X, m_F_Y - scalar.m_F_Y, m_F_Z - scalar.m_F_Z};
	}

	Point3D operator+(const Point3D scalar) const {
		return { m_F_X + scalar.m_F_X, m_F_Y + scalar.m_F_Y, m_F_Z + scalar.m_F_Z };
	}


	void normalize();

	float dotProduct(const Point3D& other) const;
};

void Point3D::normalize() {
	float length = sqrt(m_F_X * m_F_X + m_F_Y * m_F_Y + m_F_Z * m_F_Z);
	if (length != 0) {
		m_F_X /= length;
		m_F_Y /= length;
		m_F_Z /= length;
	}
}

float Point3D::dotProduct(const Point3D& other) const {
	return m_F_X * other.m_F_X + m_F_Y * other.m_F_Y + m_F_Z * other.m_F_Z;
}