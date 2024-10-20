#pragma once

class Vector4 {

public:
	float m_x, m_y, m_z, m_w;
	Vector4() : m_x(0), m_y(0), m_z(0), m_w(0) {}
	Vector4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
	Vector4(const Vector4& copy) { m_x = copy.m_x; m_y = copy.m_y; m_z = copy.m_z; m_w = copy.m_w; }

	float getX(void) const { return m_x; }
	float getY(void) const { return m_y; }
	float getZ(void) const { return m_z; }
	float getW(void) const { return m_w; }

	Vector4 normalize() {
		float length = sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
		return Vector4(m_x / length, m_y / length, m_z / length, m_w / length);

	}

	float dotProduct(const Vector4& v) const
	{
		return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z + m_w * v.m_w;
	}

	inline Vector4 operator + (const Vector4& v) const
	{
		return Vector4(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z, m_w + v.m_w);
	}

	inline Vector4 operator - (const Vector4& v) const
	{
		return Vector4(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z, m_w - v.m_w);
	}

	inline Vector4 operator * (float f) const
	{
		return Vector4(m_x * f, m_y * f, m_z * f, m_w * f);
	}	

	inline Vector4 operator / (float f) const
	{
		return Vector4(m_x / f, m_y / f, m_z / f, m_w / f);
	}

	void set(int index, float value) {
		switch (index)
		{
		case 0:
			m_x = value;
			break;
		case 1:
			m_y = value;
			break;
		case 2:
			m_z = value;
			break;
		case 3:
			m_w = value;
			break;
		default:
			break;
		}
	}

};