#include "Camera.h"

Camera::Camera()
{
	this->position = new Position(0, 0);
}

LPPosition Camera::GetPosition()
{
	return this->position;
}
