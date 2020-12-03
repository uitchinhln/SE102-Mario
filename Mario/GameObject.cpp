#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	tags = make_shared<DataTag>("root");
	id = __id++;
	InitResource();
}

DWORD64 GameObject::GetID()
{
	return id;
}

bool GameObject::IsActive()
{
	return active;
}

float& GameObject::GetGravity()
{
	return this->Gravity;
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

void GameObject::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void GameObject::CollisionDoubleFilter()
{
	if (!collisionCal) return;
	vector<shared_ptr<CollisionResult>> results = collisionCal->GetLastResults();
	for (shared_ptr<CollisionResult> coll : results) {
		if (!coll->GameColliableObject->HasCollideWith(id)) {
			coll->Remove = true;
		}
	}
	collisionCal->DropRemovedCollision();
	collisionCal->GetClampDistance();
}

bool GameObject::HasCollideWith(DWORD64 id)
{
	if (collisionCal) {
		return collisionCal->Has(id);
	}
	return true;
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
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
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

void GameObject::SetActive(bool value)
{
	this->active = value;
	if (collisionCal) {
		collisionCal->Clear();
	}
}

GameObject::~GameObject()
{
}
