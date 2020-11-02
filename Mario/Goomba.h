#pragma once
#include "GameObject.h"

enum class GoombaState {
	IDLE,
	WALK,
	DIE,
	EXPLODE
};

class Goomba :
    public GameObject,
	public enable_shared_from_this<Goomba>
{
public:
    Goomba();

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj);

	virtual void Update(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

protected:
	GoombaState state;

	Stopwatch destroyTimer;

	float GB_GRAVITY = 0.00133f * 1.5;
	float GB_SPEED = 0.20f;
	float GB_DESTROY_DELAY = 500;
	Vec2 hitbox = Vec2(46, 46);
};

