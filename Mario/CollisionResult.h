#pragma once
#include "Utils.h"
#include "IColliable.h"

class SweptCollisionResult {
public:
	float TimeToCollide;
	Direction Direction = Direction::None;
	Vec2 Distance;
	float TouchingLength = 0;

	static SweptCollisionResult Empty;
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


