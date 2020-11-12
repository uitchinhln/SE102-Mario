#pragma once
#include "GameObject.h"
class SolidBlock :
	public GameObject
{
public:
	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

	virtual void Render() override;

	static shared_ptr<SolidBlock> CreateSolidBlock(Vec2 fixPos = VECTOR_0, Vec2 size = VECTOR_0);

protected:
	Vec2 size;
};
