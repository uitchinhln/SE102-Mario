#pragma once
#include "GameObject.h"
class RaccoonLeaf :
    public GameObject,
	public enable_shared_from_this<RaccoonLeaf>
{
public:
    RaccoonLeaf();

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

protected:
	Vec2 size = Vec2(45, 45);

	Vec2 rootPos;

	int movingStep = 0;

	const int LEAF_AMPLITUDE = 48; // px
	const int LEAF_ROUNDTRIP_TIME = 800;
	const float LEAF_REVEAL_DISTANCE = 48 * 2.0f; // px
	const float LEAF_REVEAL_FORCE = 0.4f;
	const float LEAF_GRAVITY = 0.0012f;
	const float LEAF_FALLING_VEL = 0.065f;
	const float LEAF_GROWUP_SPEED = 0.0012f;
};

