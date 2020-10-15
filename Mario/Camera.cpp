#include "Camera.h"

Camera::Camera()
{
	this->position = VECTOR_0;
}

Camera::Camera(Vec2 pos, Vec2 size)
{
	this->position = Vec2(pos.x, pos.y);
	this->size = Vec2(size.x, size.y);
}

Vec2 Camera::GetPosition()
{
	return this->position;
}

Vec2 Camera::GetCamSize()
{
	return this->size;
}

void Camera::Update(int dt)
{
	this->position.x += rand() % 50;
}

