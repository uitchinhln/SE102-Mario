#pragma once
#include "Utils.h"
#include "ObjectType.h"
#include "IColliable.h"
#include "Direction.h"
#include "Animation.h"

class Koopas;

class DefaultKoopas
{
public:
	DefaultKoopas(shared_ptr<Koopas> holder);

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj);

	virtual void StatusUpdate();

	virtual void Update();

	virtual void FinalUpdate();

	virtual void Render();

	virtual ObjectType GetObjectType();

	virtual RectF GetHitBox();

	virtual float GetDamageFor(IColliable& object, Direction direction);

protected:
	weak_ptr<Koopas> holder;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	float KP_GRAVITY = 0.00133f * 1.5;
	float KP_SPEED = 0.2f;
	long KP_DESTROY_DELAY = 3000;

	Vec2 hitbox = Vec2(45, 45);
};

