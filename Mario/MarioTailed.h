#pragma once
#include "GameObject.h"
#include "Mario.h"

class MarioTailed :
	public GameObject,
	public enable_shared_from_this<MarioTailed>
{
public:
	MarioTailed(shared_ptr<Mario> holder, DWORD attackTime);

	virtual void MovingUpdate();

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

protected:
	weak_ptr<Mario> holder;
	long attackTime = 0;
	int attackState = 1;
	Stopwatch attackTimer;
};

