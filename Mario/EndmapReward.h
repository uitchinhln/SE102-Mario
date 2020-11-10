#pragma once
#include "GameObject.h"
#include "StopWatch.h"
#include "ObjectType.h"

class Mario;

enum class EndmapRewardStates {
    MUSHROOM,
    FLOWER,
    STAR
};

class EndmapReward :
    public GameObject
{
public:
	EndmapReward();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

	static shared_ptr<EndmapReward> CreateEndmapReward(Vec2 fixedPos = Vec2(100, 100));

protected:
	Vec2 size = Vec2(48, 48);

	shared_ptr<Mario> holder;

	EndmapRewardStates state;

	Stopwatch stateTimer;

	float FLY_UP_SPEED = 0.27f;
	float MARIO_RUN_SPEED = 0.28f;
};

