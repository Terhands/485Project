#include <GL/glut.h>

#include "camera.h"


const static float stepScale = 0.025f;
const static float angleScale = 5.0f;

/* 
 * Camera :: void -> Camera
 * usage :: set up a defualt camera to look along the z axis, 
 * with +y being up and positioned at the origin
 */
Camera::Camera()
{
	cameraPosition = Vector3f(0.0f, 0.0f, 0.0f);
	viewTarget = Vector3f(0.0f, 0.0f, 1.0f);
	upDirection = Vector3f(0.0f, 1.0f, 0.0f);

	xRotation = 0.0f;
	yRotation = 0.0f;
	zRotation = 0.0f;
}

/*
 * Camera :: Vector3f& * Vector3f& * Vector3f& -> Camera
 * usage :: set up the camera position, target, and orientation and make
 * sure that the view target, and up directional vectors are normalized
 */
Camera::Camera(Vector3f& _cameraPosition, Vector3f& _viewTarget, Vector3f& _upDirection)
{
	cameraPosition = _cameraPosition;
	viewTarget = _viewTarget;
	upDirection = _upDirection;

	viewTarget.normalize();
	upDirection.normalize();

	xRotation = 0.0f;
	yRotation = 0.0f;
	zRotation = 0.0f;
}

Matrix4f Camera::getRotationM()
{
	Matrix4f xRot, yRot, zRot, m_identity, result;
	m_identity = getIdentity();

	/* get the rotation radians about each axis */
	const float xRads = ToRadian(xRotation);
	const float yRads = ToRadian(yRotation);
	const float zRads = ToRadian(zRotation);

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

	result = (yRot * xRot * zRot);

    return result;
}

void Camera::setRotation(float x, float y, float z)
{
	xRotation = x;
	yRotation = y;
	zRotation = z;
}

/*
 * keyboardHandler :: int -> void
 * usage :: perform camera transformations on wasd key presses
 */
void Camera::keyboardHandler(int key)
{
	switch(key)
	{
		/* w key moves the camera forwards into the scene */
		case 'w':
		{
			Vector3f v = viewTarget;
			v *= stepScale;

			cameraPosition += v;
			break;
		}
		/* s key moves the camera forwards into the scene */
		case 's':
		{
			Vector3f v = viewTarget;
			v *= stepScale;

			cameraPosition -= v;
			break;
		}
		/* left key pans the camera left relative to the modelMatrix */
		case 'a':
		{
			// taking the cross product to get the vector perpendicular to the camera's orientation
			Vector3f lTransformation = viewTarget.cross(upDirection);
			lTransformation.normalize();

			// scaling the transformation
			lTransformation *= stepScale;

			// setting the new camera position
			cameraPosition += lTransformation;
			break;
		}
		/* right key pans the camera right relative to the modelMatrix */
		case 'd':
		{
			// taking the cross product to get the vector perpendicular to the camera's orientation
			Vector3f rTransformation = upDirection.cross(viewTarget);
			rTransformation.normalize();

			// scaling the transformation
			rTransformation *= stepScale;

			// setting the new camera position
			cameraPosition += rTransformation;
			break;
		}
	}
}

/*
 * keyboardSpecial :: int -> void
 * usage :: handles up/down camera transformations on up & down arrow key presses
 */
void Camera::keyboardSpecial(int key)
{
	switch(key)
	{
		/* up key pans the camera up relative to the modelMatrix */
		case GLUT_KEY_UP:
		{
			Vector3f v = upDirection;
			v *= stepScale;

			cameraPosition += v;
			break;
		}
		/* down key pans the camera down relative to the modelMatrix*/
		case GLUT_KEY_DOWN:
		{
			Vector3f v = upDirection;
			v *= stepScale;

			cameraPosition -= v;
			break;
		}
	}
}

/* 
 * mouseHandler :: int -> void 
 * usage :: rotate the camera around the y axis (keeping it simple)
 */
void Camera::mouseHandler(int button)
{
	switch(button)
	{
		case GLUT_RIGHT_BUTTON:
		{
			yRotation += angleScale;
			break;
		}
		case GLUT_LEFT_BUTTON:
		{
			zRotation -= angleScale;
			break;
		}
	}
}
