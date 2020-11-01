#include "IColliable.h"
#include "GameObject.h"

Vec2& IColliable::GetDistance(Vec2 position, RectF BoundingBox)
{
	return GetDistance();
}

RectF IColliable::GetHitBox(Vec2 position, RectF BoundingBox)
{
	return GetHitBox();
}

void IColliable::FixObject(shared_ptr<GameObject> object)
{
}
