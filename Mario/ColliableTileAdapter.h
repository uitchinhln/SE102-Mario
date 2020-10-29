#pragma once
#include "Utils.h"
#include "IColliable.h"
#include "ColliableTile.h"

class ColliableTileAdapter : public IColliable
{
	shared_ptr<ColliableTile> source;
	Vec2 position;
public:
	ColliableTileAdapter(shared_ptr<ColliableTile> source, Vec2 position);

	virtual Vec2& GetDistance() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;
};

