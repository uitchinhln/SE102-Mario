#include "Camera.h"
#include "Events.h"
#include "Game.h"
#include "GameObject.h"
#include "SceneManager.h"

Camera::Camera()
{
	this->Position = VECTOR_0;
	shakeTimer.Reset();
	shakeTimer.Stop();
}

Camera::Camera(Vec2 size)
{
	this->Position = VECTOR_0;
	this->size = size;
}

Vec2 Camera::GetCamSize()
{
	return size;
}

RectF Camera::GetBoundingBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
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
	if (locking || regions[activeId]->GetCameraMode() != CameraMode::TRACKING) return;

	if (shared_ptr<GameObject> obj = target.lock()) {
		RectF targetBound = obj->GetHitBox();
		float avgY = (targetBound.top + targetBound.bottom - size.y) / 2;

		Position.x = (targetBound.left - size.x / 2);
		Position.y = min(avgY + size.y / 4, max(Position.y, avgY - size.y / 4));

		Vec2 camSize = GetCamSize();
		RectF camBound = GetBoundingBox();
		RectF camLimit = activeBound;

		if (reset == 1) {
			RectF originalBound = regions[activeId]->GetBoundary();
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
	if (locking || regions[activeId]->GetCameraMode() != CameraMode::AUTOSCROLL) return;
	if (shared_ptr<GameObject> obj = target.lock()) {

		CameraRegion* region = regions[activeId];
		Position += region->GetVelocity() * CGame::Time().ElapsedGameTime;

		Vec2 camSize = GetCamSize();
		RectF camBound = GetBoundingBox();
		RectF camLimit = activeBound;

		if (reset == 1) {
			RectF originalBound = regions[activeId]->GetBoundary();
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

void Camera::Update()
{
	TrackingUpdate();
	AutoScrollUpdate();
	ShakeUpdate();
}

void Camera::Shake(int duration)
{
	shakeDuration = duration;
	shakeTimer.Start();
}

RectF Camera::GetLimitBound()
{
	return RectF();
}

void Camera::ActiveRegion(int id)
{
	if (regions.find(id) == regions.end() || regions.at(id) == nullptr) {
		return;
	}
	activeId = id;
	activeBound = regions.at(id)->GetBoundary();
}

CameraRegion* Camera::GetActiveRegion()
{
	return regions[activeId];
}

void Camera::SetLimitEdge(Direction edge, float value)
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

void Camera::ResetLimitEdge()
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

void Camera::LoadFromTMX(TiXmlElement* config)
{
	int startId = 0;

	config->QueryIntAttribute("start", &startId);

	for (TiXmlElement* node = config->FirstChildElement("Region"); node != nullptr; node = node->NextSiblingElement("Region"))
	{
		CameraRegion* region = new CameraRegion(node);
		regions[region->GetID()] = region;
	}

	ActiveRegion(startId);
}

Camera::~Camera()
{
	for each (auto iter in regions)
	{
		delete iter.second;
	}
	DebugOut(L"Camera destroyed!!!\n");
}
