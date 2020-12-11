#include "GhostBlock.h"
#include "MEntityType.h"

ObjectType GhostBlock::GetObjectType()
{
	return MEntityType::GhostBlock;
}

RectF GhostBlock::GetHitBox()
{
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
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
	block->size = size;
	return block;
}
