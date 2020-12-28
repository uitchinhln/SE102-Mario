#include "GreenMushroom.h"
#include "MEntityType.h"
#include "AnimationManager.h"

void GreenMushroom::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-1-up-mushroom");
	}
}

ObjectType GreenMushroom::GetObjectType()
{
	return MEntityType::GreenMushroom;
}

shared_ptr<GreenMushroom> GreenMushroom::CreateGreenMushroom(Vec2 pos)
{
    shared_ptr<GreenMushroom> mushroom = make_shared<GreenMushroom>();
    mushroom->SetCollisionCalculator(make_shared<CollisionCalculator>(mushroom));
    mushroom->SetPosition(pos);
    mushroom->rootPos = pos;
    return mushroom;
}
