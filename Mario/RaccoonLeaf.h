#pragma once
#include "GameObject.h"
class RaccoonLeaf :
    public GameObject,
	public enable_shared_from_this<RaccoonLeaf>
{
public:
    RaccoonLeaf();

	virtual void SetMovingStep(int value);

	virtual int GetMovingStep();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	~RaccoonLeaf();

	virtual void OnLostCamera() override;

	static shared_ptr<RaccoonLeaf> CreateRaccoonLeaf(Vec2 pos);

protected:
	Vec2 size = Vec2(44, 45);

	Vec2 rootPos;

	int movingStep = 0;

	const int LEAF_AMPLITUDE = 48; // px
	const int LEAF_ROUNDTRIP_TIME = 800;
	const float LEAF_REVEAL_DISTANCE = 48 * 1.5f; // px
	const float LEAF_REVEAL_FORCE = 0.03f;
	const float LEAF_GRAVITY = 0.0012f;
	const float LEAF_FALLING_VEL = 0.065f;
	const float LEAF_GROWUP_SPEED = 0.0012f;
};

