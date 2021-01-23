#include "TreeLeft.h"
#include "MEntityType.h"
#include "SpriteManager.h"

TreeLeft::TreeLeft()
{
	renderOrder = 500;
	sprite = NULL;
}

void TreeLeft::InitResource()
{
	if (sprite == nullptr) {
		sprite = SpriteManager::GetInstance()->Get("spr-tree-left");
	}
}

void TreeLeft::Render(D3DCOLOR overlay)
{
	InitResource();

	sprite->Draw(Position.x + size.x / 2, Position.y + size.y / 2, trans, overlay);
}

ObjectType TreeLeft::GetObjectType()
{
	return MEntityType::GhostBlock;
}

RectF TreeLeft::GetHitBox()
{
	return RectF(0, 0, 0, 0);
}

bool TreeLeft::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float TreeLeft::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<TreeLeft> TreeLeft::CreateTreeLeft(Vec2 fixPos)
{
	shared_ptr<TreeLeft> tree = make_shared<TreeLeft>();
	tree->Position = fixPos;
	return tree;
}
