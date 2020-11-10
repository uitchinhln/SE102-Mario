#include "SolidBlock.h"
#include "MEntityType.h"

ObjectType SolidBlock::GetObjectType()
{
	return MEntityType::SolidBlock;
}

RectF SolidBlock::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool SolidBlock::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float SolidBlock::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}

void SolidBlock::Render()
{
}

shared_ptr<SolidBlock> SolidBlock::CreateSolidBlock(Vec2 fixedPos, Vec2 size)
{
	shared_ptr<SolidBlock> block = make_shared<SolidBlock>();
	block->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	block->size = size;
	return block;
}
