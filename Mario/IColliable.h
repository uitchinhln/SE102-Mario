#pragma once
#include "Game.h"
#include "Direction.h"

struct SweptAABBResult;

class IColliable
{
protected:
	Vec2 Distance;

public:
	virtual Vec2& GetDistance() = 0;
	virtual RectF GetHitBox() = 0;
	virtual bool IsGetThrough(IColliable& object, Direction direction) = 0;
};

