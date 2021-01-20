#pragma once
#include "GameObject.h"
#include "VenusFireball.h"
#include "StopWatch.h"

enum class VenusState 
{
	Reveal,
	Hidden
};

class Venus :
	public GameObject,
	public enable_shared_from_this<Venus>
{
public:
	Venus();

	virtual void Reset();

	virtual void InitData();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void RestoreCollision() override;

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual void StatusUpdate() override;

	virtual void MovingUpdate();

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Venus> CreateVenus(Vec2 fixedPos);

	virtual void OnRevealed();

	virtual void OnHidden();

	virtual void OnGetInCamera() override;

protected:
	void TrackPlayerPosition();

	vector<shared_ptr<VenusFireball>> fireballs;

	Vec2 size = Vec2(16 * 3, 32 * 3);
	Vec2 rootPos;

	//bool canCollision = true;

	VenusState state = VenusState::Reveal;

	int targetLocking = 1;
	int movementState = 0;
	int verticalDirection = 1;
	Stopwatch shootTimer;
	Stopwatch movementTimer;

	int VENUS_WAIT_TIME = 1700;
	float VENUS_SPEED = 0.13f;
	float VENUS_HIDE_DISTANCE = 48 * 2;
	int VENUS_N_POOLED_BULLETS = 1;
	float VENUS_BULLET_SPEED = 0.18f;
	int VENUS_SHOOT_WAIT_TIME = VENUS_WAIT_TIME / 2;
};