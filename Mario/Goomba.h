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

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

	static shared_ptr<Goomba> CreateGoomba(Vec2 fixedPos = Vec2(100, 100));

	~Goomba();

protected:
	GoombaState state;

	Stopwatch destroyTimer;

	float GB_GRAVITY = 0.00133f * 1.5f;
	float GB_SPEED = 0.15f;
	float GB_DESTROY_DELAY = 500;
	Vec2 size = Vec2(45, 45);
	RectF hitbox;
};

