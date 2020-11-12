#pragma once
#include "Utils.h"
#include "IColliable.h"
#include "DataTag.h"

class ColliableTile : public IColliable
{
private:
	ObjectType type = ObjectType::CreateObjectType(DEFAULT_TILE_ID, DEFAULT_TILE_NAME);

protected:
	int id;

	RectF boundingBox;

	shared_ptr<DataTag> tags;

public:
	ColliableTile(int id);

	int GetID() { return this->id; }

	virtual Vec2& GetDistance() override;

	virtual RectF GetHitBox() override;

	virtual void SetHitBox(RectF r);

	virtual shared_ptr<DataTag> GetDataTag();

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override; 
	
	virtual ObjectType GetObjectType() override;
};

