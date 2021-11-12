#pragma once
#include "GameObject.h"
#include "DefaultRedGoomba.h"
#include "StopWatch.h"

enum class RedGoombaState {
	IDLE,
	WALK,
	DIE,
	EXPLODE
};

class RedGoomba :
    public GameObject
{
public:
    RedGoomba();

	virtual RedGoombaState& GetState();

	virtual Vec2& GetSize();

	virtual void SetObjectState(shared_ptr<DefaultRedGoomba> objectState);

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DXCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<RedGoomba> CreateRedGoomba(Vec2 fixedPos = Vec2(100, 100));

protected:
	shared_ptr<DefaultRedGoomba> objectState;

	RedGoombaState state;

	float GB_GRAVITY = 0.002f;
	float GB_SPEED = 0.10f;
	Vec2 size = Vec2(45, 45);
	RectF hitbox;

public:
	bool OnGround = true;
};

