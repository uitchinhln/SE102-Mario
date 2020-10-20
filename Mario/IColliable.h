#pragma once
#include "Game.h"
#include "Direction.h"

struct SweptAABBResult;

class IColliable
{
public:
	virtual Vec2 GetDistance() = 0;
	virtual RectF GetBoundingBox() = 0;
	virtual bool IsGetThrough(IColliable& object, Direction direction) = 0;
};

