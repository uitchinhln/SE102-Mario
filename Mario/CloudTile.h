#pragma once
#include "ColliableTile.h"

class CloudTile :
	public ColliableTile
{
public:
	CloudTile(int id);

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;
};

