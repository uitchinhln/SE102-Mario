#pragma once
#include "Utils.h"
#include "Transform.h"

class GameObject;

class Camera
{
protected:
	Vec2 size;
	weak_ptr<GameObject> target;
public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual RectF GetBoundingBox();

	virtual void SetTracking(weak_ptr<GameObject> target);

	virtual void Update();

	virtual void LockPosition(Vec2 pos);

	~Camera();
};

