#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "StopWatch.h"

class MarioTailed :
	public GameObject,
	public enable_shared_from_this<MarioTailed>
{
public:
	MarioTailed(shared_ptr<Mario> holder, DWORD attackTime);

	virtual void Reset();

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void MovingUpdate();

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Update() override;

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual void RestoreCollision() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	~MarioTailed();

protected:
	Vec2 size = Vec2(12, 12);
	RectF hitbox;
	weak_ptr<Mario> holder;
	long attackTime = 0;
	int attackState = -1;
	float speed = 0;
	Stopwatch attackTimer;
};

