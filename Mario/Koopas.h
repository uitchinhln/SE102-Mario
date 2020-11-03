#pragma once
#include "GameObject.h"

enum class KoopasStates {
	WALK,
	CROUCH,
	MOVING_SHELL
};

class Koopas :
    public GameObject
{
public:
	Koopas();

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;
};

