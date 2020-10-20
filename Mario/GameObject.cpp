#include "GameObject.h"

CGameObject::CGameObject()
{
	InitResource();
}

Vec2 CGameObject::GetPosition()
{
	return Position;
}

Direction CGameObject::GetDirection()
{
	return direction;
}

void CGameObject::SetDirection(Direction direction)
{
	this->direction = direction;
}

void CGameObject::InitResource()
{
}

void CGameObject::Update(vector<shared_ptr<IColliable>>* coObj)
{

}

void CGameObject::SetCollisionCalculator(unique_ptr<CollisionCalculator> calc)
{
	this->collisionCal = move(calc);
}

Vec2 CGameObject::GetDistance()
{
	return Velocity;
}

CGameObject::~CGameObject()
{
}
