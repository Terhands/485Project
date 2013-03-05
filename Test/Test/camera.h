#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "math_3d.h"

class Camera
{
private:

	Vector3f cameraPosition;
	Vector3f viewTarget;
	Vector3f upDirection;

public:

	float xRotation;
	float yRotation;
	float zRotation;

	Camera();
	Camera(Vector3f& _cameraPosition, Vector3f& _viewTarget, Vector3f& _upDirection);

	const Vector3f& getCameraPosition() const
	{
		return cameraPosition;
	}

	const Vector3f& getViewTarget() const
	{
		return viewTarget;
	}

	const Vector3f& getUpDirection() const
	{
		return upDirection;
	}

	Matrix4f getRotationM();
	void setRotation(float x, float y, float z);

	void keyboardHandler(int key);
	void keyboardSpecial(int key);
	void mouseHandler(int button);
};

#endif /* _CAMERA_H_ */