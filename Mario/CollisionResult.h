#pragma once
#include "Utils.h"
#include "Direction.h"

class GameObject;

enum class CollisionStatus {
	OUT_OF_RANGE,
	OVERLAP,
	COLLIDE,
	EMPTY
};

class SweptCollisionResult {
public:
	float TimeToCollide;
	Direction Direction = Direction::None;
	Vec2 Distance;
	float TouchingLength = 0;
	CollisionStatus Status = CollisionStatus::COLLIDE;

	static SweptCollisionResult OutOfRange;
	static SweptCollisionResult Overlap;
	static SweptCollisionResult Empty;
};

class CollisionResult
{
public:
	shared_ptr<GameObject> GameColliableObject;
	SweptCollisionResult SAABBResult;
	bool Remove = false;

	CollisionResult(SweptCollisionResult result, shared_ptr<GameObject> object);

	static bool LPCompare(const shared_ptr<CollisionResult>& a, shared_ptr<CollisionResult>& b);

	static bool Compare(const CollisionResult& a, CollisionResult& b);
};


