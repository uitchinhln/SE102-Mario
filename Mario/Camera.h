#pragma once
#include "Utils.h"
#include "Transform.h"
#include "Stopwatch.h"
#include "Direction.h"
#include "CameraRegion.h"

class GameObject;

class Camera
{
protected:
	weak_ptr<GameObject> target;

	Vec2 size;

	unordered_map<int, CameraRegion*> regions;
	int activeId = 0;

	RectF activeBound;
	int reset = 0;

	Stopwatch shakeTimer;
	int shakeDuration = 0;

	bool locking = false;
public:
	Camera();
	Camera(Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual RectF GetBoundingBox();

	virtual void SetTracking(weak_ptr<GameObject> target);

	virtual void Shake(int duration);
	virtual void ShakeUpdate();

	virtual void TrackingUpdate();
	virtual void AutoScrollUpdate();

	virtual void Update();

	virtual RectF GetLimitBound();

	virtual void ActiveRegion(int id);
	virtual CameraRegion* GetActiveRegion();

	virtual void SetLimitEdge(Direction edge, float value);
	virtual void ResetLimitEdge();

	virtual bool IsFreeze();
	virtual void SetFreeze(bool value);

	virtual void LoadFromTMX(TiXmlElement* data);

	~Camera();
};

