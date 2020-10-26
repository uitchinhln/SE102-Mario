#pragma once
#include "ColliableTile.h"

class GhostTile : 
	public ColliableTile
{
	virtual bool IsGetThrough(IColliable& object, Direction direction) override;
};

