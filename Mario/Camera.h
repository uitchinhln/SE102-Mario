#pragma once
#include "Utils.h"
#include "Transform.h"

class CGameObject;

[event_receiver(native)]
class Camera
{
protected:
	Vec2 size;
	weak_ptr<CGameObject> target;
public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual void SetTracking(weak_ptr<CGameObject> target);

	virtual void Update();

	~Camera();
};

