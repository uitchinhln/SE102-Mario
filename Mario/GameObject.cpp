#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	tags = make_shared<DataTag>("root");
	id = __id++;
	DebugOut(L"Create new game object with id = %d\n", id);
	InitResource();
}

DWORD GameObject::GetID()
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
		if (!coll->Object->HasCollideWith(id)) {
			coll->Remove = true;
		}
	}
	collisionCal->DropRemovedCollision();
	//collisionCal->GetClampDistance();
}

void GameObject::RestoreCollision()
{
	if (collisionCal == nullptr) return;

	collisionCal->RestoreCollision();
}

bool GameObject::HasCollideWith(DWORD id)
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

void GameObject::PositionUpdate()
{
	UpdatedDistance = collisionCal ? collisionCal->GetClampDistance() : Distance;
	Position += UpdatedDistance;
}

void GameObject::PositionLateUpdate()
{
	if (!collisionCal) return;
	Position -= UpdatedDistance;
	PositionUpdate();
}

void GameObject::FinalUpdate()
{
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

Vec2& GameObject::GetUpdatedDistance()
{
	return UpdatedDistance;
}

RectF GameObject::GetHitBox()
{
	return RectF(0, 0, 0, 0);
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
