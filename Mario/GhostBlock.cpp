#include "GhostBlock.h"
#include "MEntityType.h"

ObjectType GhostBlock::GetObjectType()
{
	return MEntityType::GhostBlock;
}

RectF GhostBlock::GetHitBox()
{
	return hitbox;
}

void GhostBlock::CollisionDoubleFilter()
{
}

bool GhostBlock::HasCollideWith(DWORD id)
{
	return true;
}

bool GhostBlock::IsGetThrough(GameObject& object, Direction direction)
{
	return direction != Direction::Top;
}

float GhostBlock::GetDamageFor(GameObject& object, Direction direction)
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
	block->hitbox.left = block->Position.x;
	block->hitbox.top = block->Position.y;
	block->hitbox.right = block->Position.x + size.x;
	block->hitbox.bottom = block->Position.y + size.y;
	block->size = size;
	return block;
}
