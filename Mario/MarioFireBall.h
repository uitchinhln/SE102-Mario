#pragma once
#include "GameObject.h"
#include "Mario.h"

class MarioFireBall :
	public GameObject,
	public enable_shared_from_this<MarioFireBall>
{
public:
	MarioFireBall(shared_ptr<Mario> holder);

	virtual void InitResource() override;

	virtual void Reset();

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void CollisionDoubleFilter() override;

	virtual void PositionLateUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DXCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

protected:
	Vec2 size = Vec2(24, 24);
	RectF hitbox;

	weak_ptr<Mario> holder;

	float FIREBALL_GRAVITY = 0.002f;
	float FIREBALL_JUMP_FORCE = 0.4f;
	float FIREBALL_SPEED = 0.48f;
};

