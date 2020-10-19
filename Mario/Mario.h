#pragma once
#include "Utils.h"
#include "GameObject.h"

enum MarioState;

class Mario : public CGameObject
{
public:
	Mario();

	void InitResource() override;

	void Update(vector<shared_ptr<IColliable>>* coObj) override;

	void Render() override;

	Vec2 GetDistance() override;

	RectF GetBoundingBox() override;

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

