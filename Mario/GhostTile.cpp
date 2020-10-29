#include "GhostTile.h"

GhostTile::GhostTile(int id) : ColliableTile(id)
{
}

bool GhostTile::IsGetThrough(IColliable& object, Direction direction)
{
    return direction != Direction::Top;
}
