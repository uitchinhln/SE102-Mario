#pragma once
#include "Game.h"
#include "Direction.h"

struct SweptCollisionResult;
class GameObject;

class IColliable
{
protected:
	Vec2 Distance;

public:
	virtual Vec2& GetDistance() = 0;
	virtual Vec2& GetDistance(Vec2 position, RectF BoundingBox);
	virtual RectF GetHitBox() = 0;
	virtual RectF GetHitBox(Vec2 position, RectF BoundingBox);
	virtual void FixObject(shared_ptr<GameObject> object);
	virtual bool IsGetThrough(IColliable& object, Direction direction) = 0;
	virtual float GetDamageFor(IColliable& object, Direction direction) = 0;
};

