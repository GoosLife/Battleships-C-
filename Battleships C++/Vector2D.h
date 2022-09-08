#pragma once
#include <math.h>
class Vector2D
{
public:
	Vector2D(float x, float y) : m_x(x), m_y(y) {}

	float getX() { return m_x; }
	float getY() { return m_y; }

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	float length() { return (float)sqrt(m_x * m_x + m_y * m_y); } // Explicit conversion to float to avoid warnings about conversion from double to float (C4244).
																  // Not sure why I was getting conversion errors in the first place; I am not using doubles in 
																  // this project at all.
																  // TODO: Find out when this function could possibly be handling doubles????? the fuck?

	// Operators

	Vector2D operator+ (const Vector2D& v2)
	{
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
	}

	friend Vector2D& operator+= (Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;

		return v1;
	}
	Vector2D operator* (float scalar)
	{
		return Vector2D(m_x * scalar, m_y * scalar);
	}

	Vector2D& operator*= (float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2D operator- (const Vector2D& v2)
	{
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
	}

	friend Vector2D& operator -= (Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;

		return v1;
	}

	Vector2D operator/ (float scalar)
	{
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/= (float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

	// Other vector operations

	void normalize()
	{
		float l = length();

		// Avoid divison by zero
		if (l > 0)
		{
			(*this) *= 1 / l;
		}
	}
private:
	float m_x;
	float m_y;
};