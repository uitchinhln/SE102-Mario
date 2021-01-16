#pragma once
#include "MapBlock.h"
class VoidBlock :
    public MapBlock
{
public:
	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<VoidBlock> CreateVoidBlock(Vec2 fixPos = VECTOR_0, Vec2 size = VECTOR_0);

protected:
	Vec2 size;

	RectF hitbox;
};

