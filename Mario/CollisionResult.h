#pragma once
#include "Utils.h"
#include "IColliable.h"

struct SweptCollisionResult {
	float TimeToCollide;
	Direction Direction = Direction::None;
	Vec2 Distance;

	static SweptCollisionResult Empty()
	{
		return SweptCollisionResult{ -1, Direction::None };
	}
};

class CollisionResult
{
public:
	shared_ptr<IColliable> GameColliableObject;
	SweptCollisionResult SAABBResult;

	CollisionResult(SweptCollisionResult result, shared_ptr<IColliable> object) {
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


