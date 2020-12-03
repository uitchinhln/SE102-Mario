#include "CollisionResult.h"
#include "GameObject.h"

SweptCollisionResult SweptCollisionResult::Empty = SweptCollisionResult{ -1, Direction::None };

CollisionResult::CollisionResult(SweptCollisionResult result, shared_ptr<GameObject> object)
{
	SAABBResult = result;
	GameColliableObject = object;
}

bool CollisionResult::LPCompare(const shared_ptr<CollisionResult>& a, shared_ptr<CollisionResult>& b)
{
	return a->SAABBResult.TimeToCollide < b->SAABBResult.TimeToCollide;
}

bool CollisionResult::Compare(const CollisionResult& a, CollisionResult& b)
{
	return a.SAABBResult.TimeToCollide < b.SAABBResult.TimeToCollide;
}
