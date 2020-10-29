#include "CloudTile.h"

CloudTile::CloudTile(int id) : ColliableTile(id)
{
}

bool CloudTile::IsGetThrough(IColliable& object, Direction direction)
{
    return direction != Direction::Top;
}
