#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "math_3d.h"

class Pipeline
{
private:
	struct {
		float fovy;
		float width;
		float height;
		float zNear;
		float zFar;
	} m_perspectiveProjection;

	struct {
		Vector3f cameraPos;
		Vector3f viewTarget;
		Vector3f relativeUp;
	} m_camera;

	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	Matrix4f m_identity;
	Matrix4f m_transformation;

	void initScaleTransformation(Matrix4f& m) const;
	void initRotateTransformation(Matrix4f& m) const;
	void initTranslationTransformation(Matrix4f& m) const;
	void initPerspectiveProjection(Matrix4f& m) const;
	void initCameraTransformation(Matrix4f& m) const;

public:
	Pipeline()
	{
		m_scale = Vector3f(1.0f, 1.0f, 1.0f);
		m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
		m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);

		m_identity = getIdentity();
	}

	void Scale(float scaleX, float scaleY, float scaleZ)
	{
		m_scale.x = scaleX;
		m_scale.y = scaleY;
		m_scale.z = scaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void Rotate(float rotateX, float rotateY, float rotateZ)
	{
		m_rotateInfo.x = rotateX;
		m_rotateInfo.y = rotateY;
		m_rotateInfo.z = rotateZ;
	}

	void SetPerspectiveProjection(float fovy, float width, float height, float zNear, float zFar)
	{
		m_perspectiveProjection.fovy = fovy;
		m_perspectiveProjection.width = width;
		m_perspectiveProjection.height = height;
		m_perspectiveProjection.zNear = zNear;
		m_perspectiveProjection.zFar = zFar;
	}

	void SetCamera(const Vector3f& cameraPos, const Vector3f& viewTarget, const Vector3f relativeUp)
	{
		m_camera.cameraPos = cameraPos;
		m_camera.viewTarget = viewTarget;
		m_camera.relativeUp = relativeUp;
	}

	const Matrix4f* GetTrans();
};

#endif /* _PIPELINE_H_ */