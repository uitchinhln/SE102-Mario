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
