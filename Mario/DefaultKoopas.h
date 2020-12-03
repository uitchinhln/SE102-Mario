#pragma once
#include "Utils.h"
#include "ObjectType.h"
#include "Direction.h"
#include "Animation.h"

class Koopas;
class GameObject;

class DefaultKoopas
{
public:
	DefaultKoopas();

	DefaultKoopas(shared_ptr<Koopas> koopas);

	virtual void InitResource(bool force = false);

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj);

	virtual void StatusUpdate();

	virtual void Update();

	virtual void FinalUpdate();

	virtual void Render();

	virtual ObjectType GetObjectType();

	virtual RectF GetHitBox();

	virtual float GetDamageFor(GameObject& object, Direction direction);

	~DefaultKoopas();

protected:
	weak_ptr<Koopas> koopas;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	float KP_GRAVITY = 0.00133f * 1.5f;
	float KP_SPEED = 0.18f;
	long KP_DESTROY_DELAY = 3000;

	Vec2 size = Vec2(45, 45);
	RectF hitbox;
};

