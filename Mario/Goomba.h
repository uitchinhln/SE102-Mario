#pragma once
#include "GameObject.h"

enum class GoombaState {
	IDLE,
	WALK,
	DIE
};

class Goomba :
    public GameObject
{
public:
    Goomba();

	virtual void Update(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

protected:
	GoombaState state;
};

