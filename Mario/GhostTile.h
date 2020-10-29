#pragma once
#include "ColliableTile.h"

class GhostTile : 
	public ColliableTile
{
public:
	GhostTile(int id);

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;
};

