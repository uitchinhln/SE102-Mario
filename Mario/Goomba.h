#pragma once
#include "GameObject.h"
#include "StopWatch.h"

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

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Goomba> CreateGoomba(Vec2 fixedPos = Vec2(100, 100));

	~Goomba();

#pragma region Events
	virtual void OnLostCamera() override;
#pragma endregion

protected:
	GoombaState state;

	Stopwatch destroyTimer;

	//float GB_GRAVITY = 1000.502f;
	float GB_GRAVITY = 0.002f;
	float GB_SPEED = 0.10f;
	float GB_DESTROY_DELAY = 500;
	Vec2 size = Vec2(45, 45);
	RectF hitbox;
};

