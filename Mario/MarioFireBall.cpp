#include "MarioFireBall.h"
#include "AnimationManager.h"

MarioFireBall::MarioFireBall(shared_ptr<Mario> holder)
{
	this->holder = holder;
	this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
}

void MarioFireBall::Update(vector<shared_ptr<IColliable>>* coObj)
{
}

void MarioFireBall::Render()
{
}

ObjectType MarioFireBall::GetObjectType()
{
	return MEntityType::MarioFireBall;
}

RectF MarioFireBall::GetHitBox()
{
	return RectF(this->Position.x, this->Position.y, this->Position.x + 24, this->Position.y + 24);
}

bool MarioFireBall::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float MarioFireBall::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}
