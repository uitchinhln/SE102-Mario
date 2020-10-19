#include "ColliableTile.h"

ColliableTile::ColliableTile(int id)
{
    this->id = id;
}

Vec2 ColliableTile::GetDistance()
{
    return Vec2(0, 0);
}

RectF ColliableTile::GetBoundingBox()
{
    return boundingBox;
}

void ColliableTile::SetBoundingBox(RectF r)
{
    this->boundingBox = r;
}

bool ColliableTile::IsGetThrough(IColliable& object, Direction direction)
{
    return false;
}
