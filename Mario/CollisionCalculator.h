#pragma once
#include "Utils.h"
#include "CollisionResult.h"
#include "Polygon.h"

class CollisionCalculator
{
public:
	CollisionCalculator(weak_ptr<IColliable> object);
	virtual vector<shared_ptr<CollisionResult>> CalcPotentialCollisions(vector<shared_ptr<IColliable>>* objects, bool debug = false);
	virtual vector<shared_ptr<CollisionResult>> GetLastResults();
	virtual Vec2 GetNewDistance();
	virtual Vec2 GetJet();
protected:	
	virtual SweptCollisionResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false);

	virtual bool AABB(RectF movingBounding, RectF staticBounding);

	weak_ptr<IColliable> object;
	vector<shared_ptr<CollisionResult>> results;
	Vec2 jet;
};

