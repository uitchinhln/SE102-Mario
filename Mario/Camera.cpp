#include "Camera.h"
#include "Events.h"
#include "Game.h"
#include "GameObject.h"
#include "SceneManager.h"

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

void Camera::SetTracking(weak_ptr<CGameObject> target)
{
	this->target = target;
}

void Camera::Update()
{
	if (shared_ptr<CGameObject> obj = target.lock()) {
		Position = obj->GetPosition() - size / 2;
		Position.y = obj->GetPosition().y - size.y + 27*3 + 16*3;
		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();

		if (Position.x < 0) Position.x = 0;
		if (Position.y < 0) Position.y = 0;
		if (Position.x > mapBound.x) Position.x = mapBound.x;
		if (Position.y > mapBound.y) Position.y = mapBound.y;
	}
}

void Camera::OnKeyUp(int key)
{
}

void Camera::OnKeyDown(int key)
{
	
}

Camera::~Camera()
{
	UnHookEvent();
}
