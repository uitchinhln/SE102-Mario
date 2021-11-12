#include "GameObject.h"
#include "Game.h"
#include "TextureManager.h"

GameObject::GameObject()
{
	tags = make_shared<DataTag>("root");
	id = __id++;
	//DebugOut(L"Create new game object with id = %d\n", id);
	InitResource();
}

void GameObject::Reset()
{
	active = true;

	collidable = true;

	facing = 1;
	renderOrder = 1000;

	Gravity = 0;

	Position = VECTOR_0;

	Velocity = VECTOR_0;

	Distance = VECTOR_0;
	UpdatedDistance = VECTOR_0;

	Visible = true;

	GetInCamera = false;
}

DWORD GameObject::GetID()
{
	return id;
}

int GameObject::GetRenderOrder()
{
	return renderOrder;
}

void GameObject::SetRenderOrder(int value)
{
	this->renderOrder = value;
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

	return;

	vector<shared_ptr<CollisionResult>> results = collisionCal->GetLastResults();

	RectF spR = GetHitBox();

	for each (shared_ptr<CollisionResult> var in results)
	{
		if (var->Object->IsGetThrough(*this, var->SAABBResult.Direction)) continue;
		if (this->IsGetThrough(*var->Object, var->SAABBResult.Direction)) continue;

		RectF cpR = var->Object->GetHitBox();
		Vec2 direction = ToVector(var->SAABBResult.Direction);
		
		if (collisionCal->AABB(spR, cpR)) {
			if (UpdatedDistance.x * direction.x < 0) {
				if (direction.x < 0) {
					Position.x = cpR.left - (spR.right - spR.left);
				}
				else {
					Position.x = cpR.right;
				}
			}
			else if (UpdatedDistance.y * direction.y < 0) {
				if (direction.y < 0) {
					Position.y = cpR.top - (spR.bottom - spR.top);
				}
				else {
					Position.y = cpR.bottom;
				}
			}
		}
	}
}

void GameObject::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
}

void GameObject::RenderTestBox()
{
	if (testbox.size() > 0) {
		for each (RectF box in testbox)
		{
			LPTEXTURE texture = TextureManager::GetInstance()->Get("tex-test");
			RECT r{ 0, 0, (long)box.right, (long)box.bottom };
			D3DXVECTOR3 pivot(0, 0, 0);
			CGame::GetInstance()->GetGraphic().Draw(box.left, box.top, pivot, texture, r, Transform());
			//DebugOut(L"x = %f y = %f width = %f height = %f\n", box.left, box.top, box.right, box.bottom);
		}
		testbox.clear();
	}
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

bool GameObject::Collidable()
{
	return this->collidable;
}

void GameObject::SetCollidibility(bool value)
{
	this->collidable = value;
}

GameObject::~GameObject()
{
}

void GameObject::OnLostCamera()
{
}

void GameObject::OnGetInCamera()
{
}
