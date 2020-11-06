#include "ColliableTile.h"

ColliableTile::ColliableTile(int id)
{
    this->id = id;
    this->tags = make_shared<DataTag>("ColliableTile_" + id);
}

Vec2& ColliableTile::GetDistance()
{
    return Vec2(0, 0);
}

RectF ColliableTile::GetHitBox()
{
    return boundingBox;
}

void ColliableTile::SetHitBox(RectF r)
{
    this->boundingBox = r;
}

shared_ptr<DataTag> ColliableTile::GetDataTag()
{
    return tags;
}

bool ColliableTile::IsGetThrough(IColliable& object, Direction direction)
{
    return false;
}

float ColliableTile::GetDamageFor(IColliable& object, Direction direction)
{
    return 0;
}

ObjectType ColliableTile::GetObjectType()
{
    return type;
}
