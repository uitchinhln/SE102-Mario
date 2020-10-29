#pragma once
#include "GameObject.h"
#include "Mario.h"

class MarioFireBall :
	public GameObject,
	public enable_shared_from_this<MarioFireBall>
{
public:
	MarioFireBall(shared_ptr<Mario> holder);

	virtual void Update(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

private:
	weak_ptr<Mario> holder;

	float FIREBALL_GRAVITY = 0.00193f;
	float FIREBALL_SPEED = 0.00193f;
};

