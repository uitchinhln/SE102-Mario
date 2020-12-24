#pragma once
#include "GameObject.h"
#include "VenusBullet.h"

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

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void RestoreCollision() override;

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

protected:
	vector<shared_ptr<VenusBullet>> pooledBullets;

	Vec2 size = Vec2(16 * 3, 32 * 3);

	const int VENUS_WAIT_TIME = 1700;
	const float VENUS_SPEED = 0.13f;
	const float VENUS_HIDE_DISTANCE = 48 * 2;
	const int VENUS_N_POOLED_BULLETS = 2;
	const float VENUS_BULLET_SPEED = 0.18f;
	const int VENUS_SHOOT_WAIT_TIME = VENUS_WAIT_TIME / 2;
};