#pragma once
#include "Utils.h"
#include "IColliable.h"

class ColliableTile : public IColliable
{
protected:
	int id;
	RectF boundingBox;

public:
	ColliableTile(int id);

	int GetID() { return this->id; }

	virtual Vec2 GetDistance() override;

	virtual RectF GetBoundingBox() override;
	virtual void SetBoundingBox(RectF r);

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;
};

