#pragma once
#include "Utils.h"
#include "Animation.h"
#include "Direction.h"
#include "CollisionResult.h"
#include "CollisionCalculator.h"
#include "DataTag.h"
#include "ObjectType.h"

class GameObject
{
protected:
	DWORD64 id;

	bool active = true;

	int facing = 1;

	float Gravity = 0;

	Vec2 Position;

	Vec2 Velocity = Vec2(0, 0);

	Vec2 Distance = Vec2(0, 0);

	unordered_map<string, shared_ptr<CAnimation>> animations;

	shared_ptr<CollisionCalculator> collisionCal;

	shared_ptr<DataTag> tags;

public:
	GameObject();

	DWORD64 GetID();

	virtual bool IsActive();

	virtual void SetActive(bool);

	virtual float& GetGravity();

	virtual Vec2& GetPosition();

	virtual void SetPosition(Vec2 pos);

	virtual Vec2& GetVelocity();

	virtual void SetVelocity(Vec2 velocity);

	virtual int GetFacing();

	virtual void SetFacing(int direction);

	virtual void InitResource();

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj);

	virtual void CollisionDoubleFilter();

	virtual bool HasCollideWith(DWORD64 id);

	virtual void Update();

	virtual void StatusUpdate();

	virtual void FinalUpdate();

	virtual void Render() = 0;

	virtual void SetCollisionCalculator(shared_ptr<CollisionCalculator>);

	virtual shared_ptr<CollisionCalculator> GetCollisionCalc();

	virtual shared_ptr<DataTag> GetDataTag();

	virtual Vec2& GetDistance();

	virtual RectF GetHitBox() = 0;

	virtual bool IsGetThrough(GameObject& object, Direction direction) = 0;

	virtual float GetDamageFor(GameObject& object, Direction direction) = 0;

	virtual ObjectType GetObjectType() = 0;

	~GameObject();
};

