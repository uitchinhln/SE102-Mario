#pragma once
#include "Game.h"
#include "Direction.h"
#include "ObjectType.h"

struct SweptCollisionResult;
class GameObject;

class IColliable
{
protected:
	Vec2 Distance = Vec2(0, 0);

	bool active = true;

public:
	virtual bool IsActive();
	virtual void SetActive(bool);
	virtual Vec2& GetDistance() = 0;
	virtual RectF GetHitBox() = 0;
	virtual bool IsGetThrough(IColliable& object, Direction direction) = 0;
	virtual float GetDamageFor(IColliable& object, Direction direction) = 0;
	virtual ObjectType GetObjectType() = 0;
};

