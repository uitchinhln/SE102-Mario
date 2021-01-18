#pragma once
#include "Utils.h"
#include "Transform.h"
#include "Viewport.h"
#include "Stopwatch.h"
#include "Direction.h"

class GameObject;

enum class CameraMode {
	TRACKING,
	AUTOSCROLL
};

class Camera : 
	public Viewport
{
protected:
	weak_ptr<GameObject> target;

	unordered_map<int, RectF> bounds;
	int activeId = 0;
	RectF activeBound;
	int reset = 0;

	bool autoBound = true;

	CameraMode mode = CameraMode::TRACKING;

	Stopwatch shakeTimer;
	int shakeDuration = 0;

	bool locking = false;
public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual RectF GetBoundingBox();

	virtual void SetTracking(weak_ptr<GameObject> target);

	virtual void ShakeUpdate();

	virtual void TrackingUpdate();

	virtual void AutoScrollUpdate();

	virtual void Update();

	virtual void AddBound(int id, float left, float top, float right, float bottom);

	virtual void Shake(int duration);

	virtual RectF GetActiveBound();

	virtual void SetActiveBound(int id);

	virtual void SetBoundingEdge(Direction edge, float value);

	virtual void ResetBoundingEdge();

	virtual bool IsFreeze();

	virtual void SetFreeze(bool value);

	virtual CameraMode GetCameraMode();
	
	virtual void SetCameraMode(CameraMode mode);

	~Camera();
};

