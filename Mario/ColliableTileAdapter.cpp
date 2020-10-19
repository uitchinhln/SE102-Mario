#include "ColliableTileAdapter.h"

ColliableTileAdapter::ColliableTileAdapter(shared_ptr<ColliableTile> source, Vec2 position)
{
	this->source = source;
	this->position = position;
}

Vec2 ColliableTileAdapter::GetDistance()
{
	return source->GetDistance();
}

RectF ColliableTileAdapter::GetBoundingBox()
{
	RectF sourceRect = source->GetBoundingBox();
	return RectF(sourceRect.left + position.x, 
		sourceRect.top + position.y, 
		sourceRect.right + position.x, 
		sourceRect.bottom + position.y);
}

bool ColliableTileAdapter::IsGetThrough(IColliable& object, Direction direction)
{
	return source->IsGetThrough(object, direction);
}
