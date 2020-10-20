#include "Camera.h"
#include "Events.h"
#include "Game.h"

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
}

Vec2 Camera::GetCamSize()
{
	return this->size;
}

void Camera::Update()
{
	if (CGame::GetInstance()->GetKeyBoard().IsKeyDown(DIK_RIGHT)) {
		//this->Position.x += 5;
	}
}

void Camera::OnKeyUp(int key)
{
}

void Camera::OnKeyDown(int key)
{
	if (key == DIK_RIGHT) {
		//this->Position.x += 5;
	}
}

Camera::~Camera()
{
	UnHookEvent();
}
