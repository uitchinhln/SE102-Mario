#include "Mario.h"

Mario::Mario()
{
}

void Mario::InitResource()
{
}

void Mario::Update(vector<shared_ptr<IColliable>>* coObj)
{
}

void Mario::Render()
{
}

Vec2 Mario::GetDistance()
{
	return Vec2();
}

RectF Mario::GetBoundingBox()
{
	return RectF();
}

bool Mario::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}
