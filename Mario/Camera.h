#pragma once
#include "Utils.h"
#include "Transform.h"
#include "Viewport.h"

class GameObject;

class Camera : 
	public Viewport
{
protected:
	weak_ptr<GameObject> target;
	unordered_map<int, RectF> bounds;
	int activeBound = 0;
public:
	Camera();
	Camera(Vec2 pos, Vec2 size);

	Vec2 Position;

	virtual Vec2 GetCamSize();

	virtual RectF GetBoundingBox();

	virtual void SetTracking(weak_ptr<GameObject> target);

	virtual void Update();

	virtual void AddBound(int id, float left, float top, float right, float bottom);

	virtual RectF GetActiveBound();

	virtual void SetActiveBound(int id);

	~Camera();
};

