#pragma once
#include "GameObject.h"
class Boomerang :
    public GameObject,
	public enable_shared_from_this<Boomerang>
{
public:
	Boomerang();

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
	Vec2 size = Vec2(45, 48);
	Vec2 rootPos;

	float maxY, minY = 0;

	Vec2 accelerate = VECTOR_0;

	int step = 0;

	float BOOMERANG_SPEED = 0.18f;
};

