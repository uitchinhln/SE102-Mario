#pragma once
#include "Utils.h"
#include "Animation.h"
#include "Direction.h"
#include "IColliable.h"
#include "CollisionResult.h"
#include "CollisionCalculator.h"
#include "DataTag.h"
#include "ObjectType.h"

class GameObject : public IColliable
{
protected:
	float Gravity = 0;

	Vec2 Position;

	Vec2 Velocity;

	int facing = 1;

	int state;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	shared_ptr<CollisionCalculator> collisionCal;

	shared_ptr<DataTag> tags;

public:

	GameObject();

	virtual float& GetGravity();

	virtual Vec2& GetPosition();

	virtual void SetPosition(Vec2 pos);

	virtual Vec2& GetVelocity();

	virtual void SetVelocity(Vec2 velocity);

	virtual int GetFacing();

	virtual void SetFacing(int direction);

	virtual void InitResource();

	virtual void Update(vector<shared_ptr<IColliable>>* coObj);

	virtual void Render() = 0;

	virtual int GetState() { return this->state; }

	virtual void SetState(int state) { this->state = state; }

	virtual void SetCollisionCalculator(shared_ptr<CollisionCalculator>);

	virtual shared_ptr<CollisionCalculator> GetCollisionCalc();

	virtual shared_ptr<DataTag> getDataTag();

	//Override from ICollidable
	virtual Vec2& GetDistance() override;

	virtual ObjectType GetObjectType() = 0;

	~GameObject();
};

