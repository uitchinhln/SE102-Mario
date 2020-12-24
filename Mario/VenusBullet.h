#pragma once
#include "GameObject.h"

class Venus;

class VenusBullet :
	public GameObject,
	public enable_shared_from_this<VenusBullet>
{
public:
	VenusBullet(shared_ptr<Venus> holder);

	virtual void InitResource() override;

	virtual void Reset();

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void CollisionDoubleFilter() override;

	virtual void PositionLateUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

protected:
	Vec2 size = Vec2(24, 24);
	RectF hitbox;

	weak_ptr<Venus> holder;

	float FIREBALL_GRAVITY = 0.002f;
	float FIREBALL_JUMP_FORCE = 0.4f;
	float FIREBALL_SPEED = 0.48f;
};

