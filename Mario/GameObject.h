#pragma once
#include "Utils.h"
#include "Animation.h"
#include "Direction.h"
#include "IColliable.h"
#include "CollisionResult.h"

class CGameObject : public IColliable
{
protected:
	Vec2 Position;

	Vec2 Velocity;

	int state;

	Direction direction;

	vector<shared_ptr<CAnimation>> animations;

	void CalcPotentialCollisions(vector<shared_ptr<IColliable>>* objects, vector<CollisionResult>& result);
	CollisionResult SweptAABBEx(shared_ptr<IColliable> coO);

	//Need DataTag System

public:

	CGameObject();

	virtual Vec2 GetPosition();

	virtual Direction GetDirection();

	virtual void SetDirection(Direction direction);

	virtual void InitResource();

	virtual void Update(vector<shared_ptr<IColliable>>* coObj);
	virtual void Render() = 0;

	virtual int GetState() { return this->state; }
	virtual void SetState(int state) { this->state = state; }

	//Override from ICollidable
	virtual Vec2 GetDistance() override;

	~CGameObject();
};

