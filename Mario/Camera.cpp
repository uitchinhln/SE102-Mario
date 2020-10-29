#include "Camera.h"
#include "Events.h"
#include "Game.h"
#include "GameObject.h"
#include "SceneManager.h"

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

void Camera::SetTracking(weak_ptr<GameObject> target)
{
	this->target = target;
}

void Camera::Update()
{
	if (shared_ptr<GameObject> obj = target.lock()) {
		Position.x = obj->GetPosition().x - size.x / 2;
		
		if (obj->GetPosition().y - Position.y < size.y / 4) {
			Position.y = obj->GetPosition().y - size.y / 4;
		}
		else if (obj->GetHitBox().bottom - Position.y >= size.y - 48) {
			Position.y = obj->GetHitBox().bottom - size.y + 48;
		}
		
		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound() - size;

		if (Position.x < 0) Position.x = 0;
		if (Position.y < 0) Position.y = 0;
		if (Position.x > mapBound.x) Position.x = mapBound.x;
		if (Position.y > mapBound.y) Position.y = mapBound.y;
	}
}

void Camera::LockPosition(Vec2 pos)
{
}

Camera::~Camera()
{
}
