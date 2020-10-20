#pragma once
#include "Utils.h"
#include "IColliable.h"

struct SweptAABBResult {
	float TimeToCollide;
	Direction Direction;
	Vec2 Distance;

	static SweptAABBResult Empty()
	{
		return SweptAABBResult{ -1, Direction::None };
	}
};

class CollisionResult
{
public:
	shared_ptr<IColliable> GameColliableObject;
	SweptAABBResult SAABBResult;

	CollisionResult(SweptAABBResult result, shared_ptr<IColliable> object) {
		SAABBResult = result;
		GameColliableObject = object;
	}
	static bool LPCompare(const shared_ptr<CollisionResult>& a, shared_ptr<CollisionResult>& b)
	{
		return a->SAABBResult.TimeToCollide < b->SAABBResult.TimeToCollide;
	}

	static bool Compare(const CollisionResult& a, CollisionResult& b)
	{
		return a.SAABBResult.TimeToCollide < b.SAABBResult.TimeToCollide;
	}
};


