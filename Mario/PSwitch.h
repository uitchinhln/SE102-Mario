#pragma once
#include "GameObject.h"
class PSwitch :
    public GameObject
{
public:
    PSwitch();

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

	static shared_ptr<PSwitch> CreatePSwitch(Vec2 pos);

protected:
	Vec2 size = Vec2(48, 48);

	Vec2 rootPos;

	int movingStep = 0;

	const float MUSHROOM_GROWUP_SPEED = 0.0012f;

	int CAMERA_SHAKE_DURATION = 400;

	bool Pressed = false;
};

