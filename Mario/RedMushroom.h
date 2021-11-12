#pragma once
#include "GameObject.h"

class RedMushroom :
    public GameObject,
	public enable_shared_from_this<RedMushroom>
{
public:
    RedMushroom();

	virtual void SetMovingStep(int value);

	virtual int GetMovingStep();

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

	static shared_ptr<RedMushroom> CreateRedMushroom(Vec2 pos);

protected:
	Vec2 size = Vec2(44, 45);

	Vec2 rootPos;

	int movingStep = 0;

	const float MUSHROOM_SPEED = 0.15f;
	const float MUSHROOM_GRAVITY = 0.0024f;
	const float MUSHROOM_GROWUP_SPEED = 0.0012f;
};

