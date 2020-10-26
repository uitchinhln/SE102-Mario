#include "GhostTile.h"

bool GhostTile::IsGetThrough(IColliable& object, Direction direction)
{
    return direction != Direction::Top;
}
