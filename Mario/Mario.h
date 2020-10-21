#pragma once
#include "Utils.h"
#include "GameObject.h"
#include "AbstractMarioState.h"

enum MarioState;

class Mario : public CGameObject
{
protected:
	Vec2 accelerate;

	AbstractMarioState mState;

	virtual void HookEvent();

	virtual void UnHookEvent();

	virtual void OnKeyUp(int key);

	virtual void OnKeyDown(int key);
public:
	Mario();

	void InitResource() override;

	virtual void PhysicUpdate();

	void Update(vector<shared_ptr<IColliable>>* coObj) override;

	void Render() override;

	Vec2 GetDistance() override;

	RectF GetHitBox() override;

	bool IsGetThrough(IColliable& object, Direction direction) override;
};

enum MarioState {
	IDLE_FRONT,
	IDLE,
	WALK,
	SKID,
	RUN,
	JUMP,
	HIGH_JUMP,
	DIE,
	SLIDE,
	HOLD,
	KICK,
	DIVE,
	SWIM,
	CLIMB
};

