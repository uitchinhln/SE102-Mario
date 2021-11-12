#pragma once
#include "MapBlock.h"
#include "MapProperties.h"
#include "Sprite.h"

class Pipe :
    public MapBlock
{
public:
	Pipe();

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	virtual void Render(D3DXCOLOR overlay) override;

	static shared_ptr<Pipe> CreatePipe(Vec2 pos, Vec2 size, MapProperties& props);

protected:
	Vec2 size;

	Vec2 verticalPeiceSize = Vec2(96, 48);
	Vec2 horizontalPeiceSize = Vec2(48, 96);

	int length;

	float lengthOfPiece;

	Direction direction;

	Transform trans;

	Sprite head;
	Sprite body;

	RectF hitbox;
};

