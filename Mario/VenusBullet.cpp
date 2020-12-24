#include "VenusBullet.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Venus.h"
#include "Game.h"

VenusBullet::VenusBullet(shared_ptr<Venus> holder)
{
	this->holder = holder;

	Reset();
}

void VenusBullet::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
	}
}

void VenusBullet::Reset()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	if (shared_ptr<Venus> m = holder.lock()) {
		this->Position.x = m->GetHitBox().left + size.x / 2;
		this->Position.y = m->GetHitBox().top + 30;
		this->SetFacing(m->GetFacing());

		this->Gravity = 0;
		//this->Velocity = Vec2(FIREBALL_SPEED * facing, 0); //Tinh lai theo vi tri mario

		this->Distance = Velocity * (float)dt;
	}
}

void VenusBullet::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

	RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
	if (Position.x < cam.left || Position.y < cam.top || Position.x > cam.right || Position.y > cam.bottom) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}

	for each (shared_ptr<CollisionResult> coll in coResult)
	{
		if (MEntityType::IsMario(coll->Object->GetObjectType())) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
		}
	}
}

void VenusBullet::CollisionDoubleFilter()
{
}

void VenusBullet::PositionLateUpdate()
{
}

bool VenusBullet::HasCollideWith(DWORD id)
{
	return true;
}

void VenusBullet::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;
}

void VenusBullet::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	collisionCal->Clear();
}

void VenusBullet::Render()
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	this->animations["Default"]->GetTransform()->Position = GetPosition() - cam;
	this->animations["Default"]->Render();
}

ObjectType VenusBullet::GetObjectType()
{
	return MEntityType::VenusFireBall;
}

RectF VenusBullet::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool VenusBullet::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float VenusBullet::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType()))
		return 1.0f;
	return 0.0f;
}
