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

	virtual void PositionUpdate();

	virtual void StatusUpdate();

	virtual void Update();

	virtual void FinalUpdate();

	virtual void Render(D3DCOLOR overlay);

	virtual ObjectType GetObjectType();

	virtual RectF GetHitBox();

	virtual float GetDamageFor(GameObject& object, Direction direction);

	virtual void OnDeath(Vec2 veloc);

	~DefaultKoopas();

protected:
	weak_ptr<Koopas> koopas;

	unordered_map<string, shared_ptr<CAnimation>> animations;

	float KP_GRAVITY = 0.002f;
	float KP_SPEED = 0.14f;

	Vec2 size = Vec2(45, 45);
	RectF hitbox;
};

