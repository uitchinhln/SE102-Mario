#include "TreeRight.h"
#include "SpriteManager.h"

TreeRight::TreeRight()
{
	renderOrder = 500;
}

void TreeRight::InitResource()
{
	if (sprite == nullptr) {
		sprite = SpriteManager::GetInstance()->Get("spr-tree-right");
	}
}

shared_ptr<TreeRight> TreeRight::CreateTreeRight(Vec2 fixPos)
{
	shared_ptr<TreeRight> tree = make_shared<TreeRight>();
	tree->Position = fixPos;
	tree->size = Vec2(189, 288);
	return tree;
}
