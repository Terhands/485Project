#include "Pipeline.h"

void Pipeline::initScaleTransformation(Matrix4f& m) const
{
	copyMatrix(m, m_identity);

	m.m[0][0] = m_scale.x;
	m.m[1][1] = m_scale.y;
	m.m[2][2] = m_scale.z;
}

void Pipeline::initRotateTransformation(Matrix4f& m) const
{
	Matrix4f xRot, yRot, zRot;

	/* get the rotation radians about each axis */
	const float xRads = ToRadian(m_rotateInfo.x);
	const float yRads = ToRadian(m_rotateInfo.y);
	const float zRads = ToRadian(m_rotateInfo.z);

	/* rotate around the x axis */
	copyMatrix(xRot, m_identity);
	xRot.m[1][1] = cosf(xRads); xRot.m[1][2] = -sinf(xRads);
	xRot.m[2][1] = sinf(xRads); xRot.m[2][2] = cosf(xRads);

	/* rotate around the y axis */
	copyMatrix(yRot, m_identity);
	yRot.m[0][0] = cosf(yRads); yRot.m[0][2] = -sinf(yRads);
	yRot.m[2][0] = sinf(yRads); yRot.m[2][2] = cosf(yRads);

	/* rotate around the z axis*/
	copyMatrix(zRot, m_identity);
	zRot.m[0][0] = cosf(zRads); zRot.m[0][1] = -sinf(zRads);
	zRot.m[1][0] = sinf(zRads); zRot.m[1][1] = cosf(zRads);

    m = zRot * yRot * xRot;
}

void Pipeline::initTranslationTransformation(Matrix4f& m) const
{
	copyMatrix(m, m_identity);
	m.m[0][3] = m_worldPos.x;
	m.m[1][3] = m_worldPos.y;
	m.m[2][3] = m_worldPos.z;
}

void Pipeline::initPerspectiveProjection(Matrix4f& m) const
{
	const float aRatio = m_perspectiveProjection.width / m_perspectiveProjection.height;
	const float zNear = m_perspectiveProjection.zNear;
	const float zFar = m_perspectiveProjection.zFar;
	const float zRange = zFar - zNear;
	const float tanHalfFovy = tanf(ToRadian(m_perspectiveProjection.fovy / 2.0));

	/* populate the camera's transformation matrix (converting x,y,z points to a 2D plane) */
	copyMatrix(m, m_identity);
	
	m.m[0][0] = 1.0f/(tanHalfFovy - aRatio);
	m.m[1][1] = 1.0f/(tanHalfFovy);
	m.m[2][2] = (-zNear - zFar) / zRange;
	m.m[2][3] = (2.0f*zFar*zNear)/zRange;
	m.m[3][2] = 1.0f;
	m.m[3][3] = 0.0f;
}

void Pipeline::initCameraTransformation(Matrix4f& m) const
{
	Matrix4f mRot, mTrans;

	/* stage 1 of UVN camera transformation: compute the rotation matrix */
	Vector3f N = m_camera.viewTarget;
	Vector3f U = m_camera.relativeUp;
	Vector3f V;

	/* make sure N & U are normalized */
	N.normalize();
	U.normalize();

	/* making sure the coordinate system is valid (perpendicular axes) */
	U = U.cross(N);
	V = N.cross(U);

	/* setting up the camera's tranform matrix */
	mRot.m[0][0] = U.x; mRot.m[0][1] = U.y; mRot.m[0][2] = U.z; mRot.m[0][3] = 0.0f;
	mRot.m[1][0] = V.x; mRot.m[1][1] = V.y; mRot.m[1][2] = V.z; mRot.m[1][3] = 0.0f;
	mRot.m[2][0] = N.x; mRot.m[2][1] = N.y; mRot.m[2][2] = N.z; mRot.m[2][3] = 0.0f;
	mRot.m[3][0] = 0.0f; mRot.m[3][1] = 0.0f; mRot.m[3][2] = 0.0f; mRot.m[3][3] = 1.0f;

	/* stage 2 of UVN camera transformation: compute the translation matrix back to the origin */
	copyMatrix(mTrans, m_identity);
	mTrans.m[0][3] = -m_camera.cameraPos.x;
	mTrans.m[1][3] = -m_camera.cameraPos.y;
	mTrans.m[2][3] = -m_camera.cameraPos.z;

	/* compute the final tranformation matrix for the camera coordinate system */
	m = mTrans * mRot;
}

const Matrix4f* Pipeline::GetTrans()
{
	Matrix4f sTransform, rTransform, tTransform, pTransform, cTransform;

	initScaleTransformation(sTransform);
	initRotateTransformation(rTransform);
	initTranslationTransformation(tTransform);
	initPerspectiveProjection(pTransform);
	initCameraTransformation(cTransform);

	m_transformation = pTransform * cTransform * tTransform * rTransform * sTransform;

	return &m_transformation;
}
