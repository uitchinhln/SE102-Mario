#pragma once
#include "AnimationManager.h"
#include "Direction.h"
#include "ObjectType.h"

class RedGoomba;
class GameObject;

class DefaultRedGoomba
{
public:
	DefaultRedGoomba(shared_ptr<RedGoomba> holder);

	virtual void InitResource();

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj);

	virtual void PositionUpdate();

	virtual void StatusUpdate();

	virtual void Update();

	virtual void FinalUpdate();

	virtual void Render();

	virtual ObjectType GetObjectType();

	virtual bool IsGetThrough(GameObject& object, Direction direction);

	virtual float GetDamageFor(GameObject& object, Direction direction);

protected:
	weak_ptr<RedGoomba> holder;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	float GB_GRAVITY = 0.002f;
	float GB_SPEED = 0.10f;
	float GB_DESTROY_DELAY = 500;
};

