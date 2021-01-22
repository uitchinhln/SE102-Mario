#pragma once
#include "MapBlock.h"
class TreeLeft :
    public MapBlock
{
public:
	virtual void InitResource() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<TreeLeft> CreateTreeLeft(Vec2 fixPos = VECTOR_0);

protected:
	Vec2 size = Vec2(192, 192);

	Sprite sprite;

	Transform trans;
};

