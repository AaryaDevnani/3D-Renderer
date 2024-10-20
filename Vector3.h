#pragma once

class Vector3 {

public:
	float m_x, m_y, m_z;
	Vector3() : m_x(0), m_y(0), m_z(0) {}
	Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
	Vector3(const Vector3& copy) { m_x = copy.m_x; m_y = copy.m_y; m_z = copy.m_z; }

	float getX(void) const { return m_x; }
	float getY(void) const { return m_y; }
	float getZ(void) const { return m_z; }

	Vector3 crossProduct(const Vector3& v)
	{
		return Vector3(
			m_y * v.m_z - m_z * v.m_y,
			m_z * v.m_x - m_x * v.m_z,
			m_x * v.m_y - m_y * v.m_x);
	}

	float dotProduct(const Vector3& v)
	{
		return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
	}
	Vector3 normalize()
	{
		float length = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		return Vector3(m_x / length, m_y / length, m_z / length);
	}
};

inline Vector3 operator + (const Vector3& v0, const Vector3& v1)
{
	return Vector3(v0.m_x + v1.m_x, v0.m_y + v1.m_y, v0.m_z + v1.m_z);
}

inline Vector3 operator - (const Vector3& v0, const Vector3& v1)
{
	return Vector3(v0.m_x - v1.m_x, v0.m_y - v1.m_y, v0.m_z - v1.m_z);
}

inline Vector3 operator * (const Vector3& v, float f)
{
	return Vector3(v.m_x * f, v.m_y * f, v.m_z * f);
}

inline Vector3 operator * (Vector3& v, const Vector3& v2)
{
	return Vector3(v.m_x * v2.m_x, v.m_y * v2.m_y, v.m_z * v2.m_z);
	
}


