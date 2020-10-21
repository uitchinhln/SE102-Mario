#pragma once
#include "Utils.h"
#include "Animation.h"
#include "Direction.h"
#include "IColliable.h"
#include "CollisionResult.h"
#include "CollisionCalculator.h"
#include "DataTag.h"

class CGameObject : public IColliable
{
protected:
	Vec2 Position;

	Vec2 Velocity;

	int state;

	Direction direction;

	vector<shared_ptr<CAnimation>> animations;

	unique_ptr<CollisionCalculator> collisionCal;

	shared_ptr<DataTag> tags;

public:

	CGameObject();

	virtual Vec2 GetPosition();

	virtual Direction GetDirection();

	virtual void SetDirection(Direction direction);

	virtual void InitResource();

	virtual void PhysicUpdate();
	virtual void Update(vector<shared_ptr<IColliable>>* coObj);

	virtual void Render() = 0;

	virtual int GetState() { return this->state; }
	virtual void SetState(int state) { this->state = state; }

	virtual void SetCollisionCalculator(unique_ptr<CollisionCalculator>);

	virtual shared_ptr<DataTag> getDataTag();

	//Override from ICollidable
	virtual Vec2 GetDistance() override;

	~CGameObject();
};

