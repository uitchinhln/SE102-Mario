#pragma once
#include "Utils.h"
#include "CollisionResult.h"
#include "Polygon.h"

class CollisionCalculator
{
	weak_ptr<IColliable> object;
	vector<shared_ptr<CollisionResult>> results;
	Vec2 jet;
public:
	CollisionCalculator(weak_ptr<IColliable> object);
	vector<shared_ptr<CollisionResult>> CalcPotentialCollisions(vector<shared_ptr<IColliable>>* objects, bool debug = false);
	Vec2 GetNewDistance();
	Vec2 GetJet();
protected:	
	SweptCollisionResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false);

	bool AABB(RectF movingBounding, RectF staticBounding);

	SweptCollisionResult SweptSAT(CPolygon movingBounding, Vec2 distance, CPolygon staticBounding, bool debug = false);
};

