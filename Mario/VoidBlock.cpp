#include "VoidBlock.h"
#include "MEntityType.h"

ObjectType VoidBlock::GetObjectType()
{
	return MEntityType::VoidBlock;
}

RectF VoidBlock::GetHitBox()
{
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
	return hitbox;
}

bool VoidBlock::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float VoidBlock::GetDamageFor(GameObject& object, Direction direction)
{
	return 99999.0f;
}

shared_ptr<VoidBlock> VoidBlock::CreateVoidBlock(Vec2 fixPos, Vec2 size)
{
	shared_ptr<VoidBlock> block = make_shared<VoidBlock>();
	block->Reset();
	block->SetPosition(Vec2(fixPos.x, fixPos.y));
	block->size = size;
	return block;
}
