#include "Camera.h"
#include "InputProcessor.h"
#include "Events.h"

void Camera::HookEvent()
{
	__hook(&Events::KeyDownEvent, Events::GetInstance(), &Camera::OnKeyDown);
	__hook(&Events::KeyUpEvent, Events::GetInstance(), &Camera::OnKeyUp);
}

void Camera::UnHookEvent()
{
	__unhook(&Events::KeyDownEvent, Events::GetInstance(), &Camera::OnKeyDown);
	__unhook(&Events::KeyUpEvent, Events::GetInstance(), &Camera::OnKeyUp);
}

Camera::Camera()
{
	this->Position = VECTOR_0;
}

Camera::Camera(Vec2 pos, Vec2 size)
{
	this->Position = Vec2(pos.x, pos.y);
	this->size = Vec2(size.x, size.y);
	HookEvent();
}

Vec2 Camera::GetCamSize()
{
	return this->size;
}

void Camera::Update(int dt)
{
	//this->Position.x += 0.07*dt;
}

void Camera::OnKeyUp(int key)
{
	this->Position.x += 5;
}

void Camera::OnKeyDown(int key)
{
}

Camera::~Camera()
{
	UnHookEvent();
}
