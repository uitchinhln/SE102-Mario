#include "GhostBlock.h"
#include "MEntityType.h"

ObjectType GhostBlock::GetObjectType()
{
	return MEntityType::GhostBlock;
}

RectF GhostBlock::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool GhostBlock::IsGetThrough(IColliable& object, Direction direction)
{
	return direction != Direction::Top;
}

float GhostBlock::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}

void GhostBlock::Render()
{
}

shared_ptr<GhostBlock> GhostBlock::CreateGhostBlock(Vec2 fixedPos, Vec2 size)
{
	shared_ptr<GhostBlock> block = make_shared<GhostBlock>();
	block->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	block->size = size;
	return block;
}
