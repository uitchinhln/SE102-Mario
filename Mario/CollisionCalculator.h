#pragma once
#include "Utils.h"
#include "CollisionResult.h"
#include "Polygon.h"

class GameObject;

class CollisionCalculator
{
public:
	CollisionCalculator(weak_ptr<GameObject> object);
	virtual vector<shared_ptr<CollisionResult>> CalcPotentialCollisions(vector<shared_ptr<GameObject>>* objects, bool debug = false);
	virtual vector<shared_ptr<CollisionResult>> GetLastResults();
	virtual vector<shared_ptr<CollisionResult>> GetLast_Results();
	virtual Vec2 GetClampDistance();
	virtual Vec2 GetJet();
	virtual void Clear();
	virtual void DropRemovedCollision();
	virtual bool AABB(RectF movingBounding, RectF staticBounding);
	virtual bool BroadPhase(RectF movingBounding, Vec2 distance, RectF staticBounding);
	virtual bool Has(DWORD id);

protected:	
	virtual SweptCollisionResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false);

	weak_ptr<GameObject> object;
	vector<shared_ptr<CollisionResult>> results;
	unordered_map<DWORD, shared_ptr<CollisionResult>> key_results;
	vector<shared_ptr<CollisionResult>> _results;
	Vec2 jet;
};

