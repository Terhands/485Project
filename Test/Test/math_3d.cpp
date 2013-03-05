#include "math_3d.h"

/* 
 * Cross :: const Vector3f& -> Vector3f 
 * usage :: compute the cross product of this & v
 * Formula: (a,b,c) X (d,e,f) = (bf-ce, cd-af, ae-bd)
 */
Vector3f Vector3f::cross(const Vector3f& v) const
{
	float xResult = y * v.z - z * v.y;
	float yResult = z * v.x - x * v.z;
	float zResult = x * v.y - y * v.x;

	return Vector3f(xResult, yResult, zResult);
}

/*
 * Normalize :: void -> Vector3f&
 * usage :: convert this Vector to have magnitude = 1 (be a unit vector)
 */
Vector3f& Vector3f::normalize()
{
	float magnitude = sqrtf(x * x + y * y + z * z);

	x = x / magnitude;
	y = y / magnitude;
	z = z / magnitude;

	return *this;
}

/*
 * copyMatrix :: Matrix4f& * Matrix4f -> void
 * usage :: copies the values of b into a
 */
void copyMatrix(Matrix4f& a, const Matrix4f b)
{
	for(int i = 0; i < 4; i++)
	{	
		for(int j = 0; j < 4; j++)
		{
			a.m[i][j] = b.m[i][j];
		}
	}
}

Matrix4f getIdentity()
{
	Matrix4f m_identity;

	m_identity.m[0][0] = 1.0f; m_identity.m[1][0] = 0.0f; m_identity.m[2][0] = 0.0f; m_identity.m[3][0] = 0.0f; 
	m_identity.m[0][1] = 0.0f; m_identity.m[1][1] = 1.0f; m_identity.m[2][1] = 0.0f; m_identity.m[3][1] = 0.0f; 
	m_identity.m[0][2] = 0.0f; m_identity.m[1][2] = 0.0f; m_identity.m[2][2] = 1.0f; m_identity.m[3][2] = 0.0f; 
	m_identity.m[0][3] = 0.0f; m_identity.m[1][3] = 0.0f; m_identity.m[2][3] = 0.0f; m_identity.m[3][3] = 1.0f; 
	
	return m_identity;
}
