#pragma once
#include "GameObject.h"

class VenusFireball :
	public GameObject,
	public enable_shared_from_this<VenusFireball>
{
public:
	VenusFireball();

	virtual void InitResource() override;

	virtual void Reset(RectF hitbox, int facing);

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void CollisionDoubleFilter() override;

	virtual void PositionLateUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DXCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	virtual void OnLostCamera();

protected:
	Vec2 size = Vec2(24, 24);
	RectF hitbox;
	Vec2 rootPos;

	float FIREBALL_SPEED = 0.18f;
};

