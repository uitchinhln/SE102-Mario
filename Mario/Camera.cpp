#include "Camera.h"
#include "Events.h"
#include "Game.h"
#include "GameObject.h"
#include "SceneManager.h"

Camera::Camera() : Viewport(VECTOR_0, VECTOR_0)
{
	this->Position = VECTOR_0;
	shakeTimer.Reset();
	shakeTimer.Stop();
}

Camera::Camera(Vec2 pos, Vec2 size) : Viewport(pos, size)
{
}

Vec2 Camera::GetCamSize()
{
	return Vec2((float)this->d3dvp.Width, (float)this->d3dvp.Height);
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
	if (locking) return;
	if (shared_ptr<GameObject> obj = target.lock()) {
		RectF targetBound = obj->GetHitBox();

		Position.x = targetBound.left - this->d3dvp.Width / 2;
		Position.y = targetBound.top - this->d3dvp.Height / 2;

		if (shakeTimer.IsRunning()) {
			if (shakeTimer.Elapsed() < shakeDuration) {
				Position.x += rand() % 14;
				Position.x -= rand() % 10;

				Position.y += rand() % 14;
				Position.y -= rand() % 10;
			}
			else {
				shakeTimer.Stop();
			}
		}
		
		Vec2 camSize = GetCamSize();
		RectF camBound = GetBoundingBox();
		RectF camLimit = bounds.at(activeBound);

		if (camBound.left < camLimit.left) Position.x = camLimit.left;
		if (camBound.top < camLimit.top) Position.y = camLimit.top;
		if (camBound.right > camLimit.right) Position.x = camLimit.right - camSize.x;
		if (camBound.bottom > camLimit.bottom) Position.y = camLimit.bottom - camSize.y;
	}
}

void Camera::AddBound(int id, float left, float top, float right, float bottom)
{
	this->bounds[id] = RectF(left, top, right, bottom);
	if (bounds.find(activeBound) == bounds.end()) {
		activeBound = id;
	}
}

void Camera::Shake(int duration)
{
	shakeDuration = duration;
	shakeTimer.Start();
}

RectF Camera::GetActiveBound()
{
	return bounds.at(activeBound);
}

void Camera::SetActiveBound(int id)
{
	if (bounds.find(id) != bounds.end()) {
		activeBound = id;
	}
}

bool Camera::IsLocking()
{
	return locking;
}

void Camera::SetLocking(bool value)
{
	this->locking = value;
}

Camera::~Camera()
{
	DebugOut(L"Camera destroyed!!!");
}
