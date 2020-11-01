#include "Goomba.h"
#include "MEntityType.h"
#include "AnimationManager.h"

Goomba::Goomba()
{
	this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
}

void Goomba::Update(vector<shared_ptr<IColliable>>* coObj)
{
}

void Goomba::Render()
{
}

ObjectType Goomba::GetObjectType()
{
	return MEntityType::Goomba;
}

RectF Goomba::GetHitBox()
{
	return RectF();
}

bool Goomba::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float Goomba::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}
