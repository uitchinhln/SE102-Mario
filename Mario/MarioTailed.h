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

	virtual void CollisionDoubleFilter() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void MovingUpdate();

	virtual void Update() override;

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

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

