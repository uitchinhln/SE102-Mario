#include "Camera.h"
#include "Events.h"
#include "Game.h"
#include "GameObject.h"
#include "SceneManager.h"

Camera::Camera() : Viewport(VECTOR_0, VECTOR_0)
{
	this->Position = VECTOR_0;
}

Camera::Camera(Vec2 pos, Vec2 size) : Viewport(pos, size)
{
}

Vec2 Camera::GetCamSize()
{
	return Vec2(this->d3dvp.Width, this->d3dvp.Height);
}

RectF Camera::GetBoundingBox()
{
	return RectF(Position.x, Position.y, Position.x + this->d3dvp.Width, Position.y + this->d3dvp.Height);
}

void Camera::SetTracking(weak_ptr<GameObject> target)
{
	this->target = target;
}

void Camera::Update()
{
	if (shared_ptr<GameObject> obj = target.lock()) {
		Position.x = obj->GetPosition().x - this->d3dvp.Width / 2;
		
		if (obj->GetPosition().y - Position.y < this->d3dvp.Height / 4) {
			Position.y = obj->GetPosition().y - this->d3dvp.Height / 4;
		}
		else if (obj->GetHitBox().bottom - Position.y >= this->d3dvp.Height - 48) {
			Position.y = obj->GetHitBox().bottom - this->d3dvp.Height + 48;
		}
		
		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound() - GetCamSize();

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
