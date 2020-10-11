#include "Camera.h"

Camera::Camera()
{
	this->transform = new Transform;
}

Transform* Camera::GetTransform()
{
	return this->transform;
}

