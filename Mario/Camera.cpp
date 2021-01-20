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

void Camera::ShakeUpdate()
{
	if (shakeTimer.IsRunning()) {
		if (shakeTimer.Elapsed() < shakeDuration) {
			Position.x += rand() % 8;
			Position.x -= rand() % 8;

			Position.y += rand() % 8;
			Position.y -= rand() % 8;
		}
		else {
			shakeTimer.Stop();
		}
	}
}

void Camera::TrackingUpdate()
{
	if (locking || mode != CameraMode::TRACKING) return;

	if (shared_ptr<GameObject> obj = target.lock()) {
		RectF targetBound = obj->GetHitBox();
		float avgY = (targetBound.top + targetBound.bottom - this->d3dvp.Height) / 2;

		Position.x = (targetBound.left - this->d3dvp.Width / 2);
		Position.y = min(avgY + this->d3dvp.Height / 4, max(Position.y, avgY - this->d3dvp.Height / 4));

		Vec2 camSize = GetCamSize();
		RectF camBound = GetBoundingBox();
		RectF camLimit = activeBound;

		if (reset == 1) {
			RectF originalBound = bounds.at(activeId);
			if (camBound.left >= originalBound.left && camBound.right <= originalBound.right
				&& camBound.top >= originalBound.top && camBound.bottom <= originalBound.bottom)
			{
				camLimit = activeBound = originalBound;
				reset = 2;
			}
		}

		if (camBound.left < camLimit.left) Position.x = camLimit.left;
		if (camBound.top < camLimit.top) Position.y = camLimit.top;
		if (camBound.right > camLimit.right) Position.x = camLimit.right - camSize.x;
		if (camBound.bottom > camLimit.bottom) Position.y = camLimit.bottom - camSize.y;
	}
}

void Camera::AutoScrollUpdate()
{
	if (locking || mode != CameraMode::AUTOSCROLL) return;

}

void Camera::Update()
{
	TrackingUpdate();
	AutoScrollUpdate();
	ShakeUpdate();
}

void Camera::AddBound(int id, float left, float top, float right, float bottom)
{
	this->bounds[id] = RectF(left, top, right, bottom);
	if (bounds.find(activeId) == bounds.end()) {
		activeId = id;
		activeBound = RectF(left, top, right, bottom);
	}
}

void Camera::Shake(int duration)
{
	shakeDuration = duration;
	shakeTimer.Start();
}

RectF Camera::GetActiveBound()
{
	return bounds.at(activeId);
}

void Camera::SetActiveBound(int id)
{
	if (bounds.find(id) != bounds.end()) {
		activeId = id;
		activeBound = bounds.at(id);
	}
}

void Camera::SetBoundingEdge(Direction edge, float value)
{
	reset = 0;
	switch (edge)
	{
	case Direction::Left:
		activeBound.left = value;
		break;
	case Direction::Top:
		activeBound.top = value;
		break;
	case Direction::Right:
		activeBound.right = value;
		break;
	case Direction::Bottom:
		activeBound.bottom = value;
		break;
	}
}

void Camera::ResetBoundingEdge()
{
	if (!reset) reset = 1;
}

bool Camera::IsFreeze()
{
	return locking;
}

void Camera::SetFreeze(bool value)
{
	this->locking = value;
}

CameraMode Camera::GetCameraMode()
{
	return this->mode;
}

void Camera::SetCameraMode(CameraMode mode)
{
	this->mode = mode;
}

Camera::~Camera()
{
	DebugOut(L"Camera destroyed!!!");
}
