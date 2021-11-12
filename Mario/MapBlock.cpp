#include "MapBlock.h"

MapBlock::MapBlock()
{
	Reset();
}

void MapBlock::Update()
{
}

void MapBlock::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void MapBlock::PositionUpdate()
{
}

void MapBlock::RestoreCollision()
{
}

void MapBlock::PositionLateUpdate()
{
}

void MapBlock::CollisionDoubleFilter()
{
}

void MapBlock::StatusUpdate()
{
}

void MapBlock::FinalUpdate()
{
    //this->active = true;
}

bool MapBlock::HasCollideWith(DWORD id)
{
    return true;
}

void MapBlock::Render(D3DXCOLOR overlay)
{
}
