#pragma once
#include "Utils.h"
#include "CollisionResult.h"

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
	SweptAABBResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false);
};

