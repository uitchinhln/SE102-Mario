#include "GameObject.h"

GameObject::GameObject()
{
	tags = make_shared<DataTag>("root");
	InitResource();
}

float& GameObject::GetGravity()
{
	return this->Gravity;
	// TODO: insert return statement here
}

Vec2& GameObject::GetPosition()
{
	return Position;
}

void GameObject::SetPosition(Vec2 pos)
{
	this->Position = pos;
}

Vec2& GameObject::GetVelocity()
{
	return Velocity;
}

void GameObject::SetVelocity(Vec2 velocity)
{
	this->Velocity = velocity;
}

int GameObject::GetFacing()
{
	return this->facing;
}

void GameObject::SetFacing(int direction)
{
	this->facing = -1;
	if (direction >= 0) this->facing = 1;
}

void GameObject::InitResource()
{
}

void GameObject::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
}

void GameObject::Update()
{

}

void GameObject::StatusUpdate()
{
}

void GameObject::FinalUpdate()
{
	if (collisionCal) {
		Distance = collisionCal->GetClampDistance();
	}
	Position += Distance;
	Distance = Velocity * CGame::Time().ElapsedGameTime;
}

void GameObject::SetCollisionCalculator(shared_ptr<CollisionCalculator> calc)
{
	this->collisionCal = calc;
}

shared_ptr<CollisionCalculator> GameObject::GetCollisionCalc()
{
	return this->collisionCal;
}

shared_ptr<DataTag> GameObject::GetDataTag()
{
	return tags;
}

Vec2& GameObject::GetDistance()
{
	return Distance;
}

GameObject::~GameObject()
{
}
