#include "GameObject.h"

CGameObject::CGameObject()
{
	InitResource();
}

float& CGameObject::GetGravity()
{
	return this->Gravity;
	// TODO: insert return statement here
}

Vec2& CGameObject::GetPosition()
{
	return Position;
}

void CGameObject::SetPosition(Vec2 pos)
{
	this->Position = pos;
}

Vec2& CGameObject::GetVelocity()
{
	return Velocity;
}

void CGameObject::SetVelocity(Vec2 velocity)
{
	this->Velocity = velocity;
}

int CGameObject::GetFacing()
{
	return this->facing;
}

void CGameObject::SetFacing(int direction)
{
	this->facing = -1;
	if (direction >= 0) this->facing = 1;
}

void CGameObject::InitResource()
{
}

void CGameObject::Update(vector<shared_ptr<IColliable>>* coObj)
{

}

void CGameObject::SetCollisionCalculator(shared_ptr<CollisionCalculator> calc)
{
	this->collisionCal = calc;
}

shared_ptr<CollisionCalculator> CGameObject::GetCollisionCalc()
{
	return this->collisionCal;
}

shared_ptr<DataTag> CGameObject::getDataTag()
{
	return tags;
}

Vec2& CGameObject::GetDistance()
{
	return Distance;
}

CGameObject::~CGameObject()
{
}
