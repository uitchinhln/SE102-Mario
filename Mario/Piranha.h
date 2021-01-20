#pragma once
#include "GameObject.h"
#include "StopWatch.h"

enum class PiranhaState
{
	Reveal,
	Hidden
};

class Piranha :
    public GameObject,
	public enable_shared_from_this<Piranha>
{
public:
    Piranha();

	void Reset();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void RestoreCollision() override;

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Piranha> CreatePiranha(Vec2 fixedPos);

	virtual void OnRevealed();

	virtual void OnHidden();

	virtual void OnGetInCamera() override;

protected:
	void TrackPlayerPosition();

	Vec2 size = Vec2(16 * 3, 23 * 3);
	Vec2 rootPos;

	//bool canCollision = true;

	PiranhaState state = PiranhaState::Reveal;

	int movementState = 0;
	Stopwatch movementTimer;

	int VENUS_WAIT_TIME = 1700;
	float VENUS_SPEED = 0.13f;
	float VENUS_HIDE_DISTANCE = 48 * 2;
};

