#include "SolidBlock.h"
#include "MEntityType.h"

ObjectType SolidBlock::GetObjectType()
{
	return MEntityType::SolidBlock;
}

RectF SolidBlock::GetHitBox()
{
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
	return hitbox;
}

bool SolidBlock::IsGetThrough(GameObject& object, Direction direction)
{
	return false;
}

float SolidBlock::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<SolidBlock> SolidBlock::CreateSolidBlock(Vec2 fixedPos, Vec2 size)
{
	shared_ptr<SolidBlock> block = make_shared<SolidBlock>();
	block->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	block->size = size;
	return block;
}
