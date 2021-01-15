#pragma once
#include "Utils.h"
#include "CollisionResult.h"

class GameObject;

class CollisionCalculator
{
public:
	CollisionCalculator(weak_ptr<GameObject> object);
	virtual vector<shared_ptr<CollisionResult>> CalcPotentialCollisions(vector<shared_ptr<GameObject>>* objects, bool debug = false);
	virtual void RestoreCollision();
	virtual vector<shared_ptr<CollisionResult>> GetLastResults();
	virtual vector<shared_ptr<CollisionResult>> GetOverlaps();
	virtual int HasOverlapped();
	virtual Vec2 GetClampDistance();
	virtual Vec2 GetJet();
	virtual void Clear();
	virtual void DropRemovedCollision();
	virtual bool AABB(RectF movingBounding, RectF staticBounding);
	static bool CommonAABB(RectF movingBounding, RectF staticBounding);
	virtual bool BroadPhase(RectF movingBounding, Vec2 distance, RectF staticBounding);
	virtual SweptCollisionResult SweptAABB(RectF movingBounding, Vec2 distance, RectF staticBounding, bool debug = false);
	virtual bool Has(DWORD id);

protected:	
	weak_ptr<GameObject> object;
	vector<shared_ptr<CollisionResult>> results;
	unordered_map<DWORD, shared_ptr<CollisionResult>> key_results;
	vector<shared_ptr<CollisionResult>> _results;
	vector<shared_ptr<CollisionResult>> overlaps;
	Vec2 jet;
};

