#ifndef _MATH3D_H_
#define _MATH3D_H_

#define _USE_MATH_DEFINES
#include <cmath>

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

struct Vector2f
{
	float x;
	float y;

	Vector2f() {}

	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f() {}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f& operator+=(const Vector3f& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vector3f& operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	/* compute the cross product of this vector and v */
	Vector3f cross(const Vector3f& v) const;

	/* normalize this Vector (into a unit vector) */
	Vector3f& normalize();
};

struct Matrix4f
{
	float m[4][4];

	inline Matrix4f operator*(const Matrix4f& rightOperand) const
	{
		Matrix4f result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				// matrix multiplication row * column for each row*col combination
				result.m[i][j] = m[i][0] * rightOperand.m[0][j] +
								 m[i][1] * rightOperand.m[1][j] +
								 m[i][2] * rightOperand.m[2][j] +
								 m[i][3] * rightOperand.m[3][j];
			}
		}
		return result;
	}
};

/*
 * copyMatrix :: Matrix4f& * Matrix4f -> void
 * usage :: copies the values of b into a
 */
void copyMatrix(Matrix4f& a, const Matrix4f b);

Matrix4f getIdentity();


#endif /* _MATH3D_H_ */