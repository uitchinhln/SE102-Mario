#include "MarioFireBall.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "MEntityType.h"
#include "Game.h"
#include "EffectServer.h"
#include "SmokeSpotFX.h"

MarioFireBall::MarioFireBall(shared_ptr<Mario> holder)
{
	this->holder = holder;

	Reset();
}

void MarioFireBall::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
	}
}

void MarioFireBall::Reset()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	if (shared_ptr<Mario> m = holder.lock()) {
		this->Position.x = m->GetHitBox().left + size.x / 2;
		this->Position.y = m->GetHitBox().top + 30;
		this->SetFacing(m->GetFacing());

		this->Gravity = FIREBALL_GRAVITY;
		this->Velocity = Vec2(FIREBALL_SPEED * facing, 0);

		this->Distance = Velocity * (float)dt;
	}
}

void MarioFireBall::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	vector<shared_ptr<GameObject>> objs;
	for each (shared_ptr<GameObject> var in *coObj)
	{
		if (MEntityType::IsEnemy(var->GetObjectType()) || MEntityType::IsTile(var->GetObjectType())) {
			objs.push_back(var);
		}
	}
	collisionCal->CalcPotentialCollisions(&objs);
}

void MarioFireBall::CollisionDoubleFilter()
{
}

void MarioFireBall::PositionLateUpdate()
{
}

bool MarioFireBall::HasCollideWith(DWORD id)
{
	return true;
}

void MarioFireBall::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;
}

void MarioFireBall::StatusUpdate()
{
	vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

	// No collision occured, proceed normally
	if (coResult.size() > 0) {
		Vec2 jet = collisionCal->GetJet();

		if (jet.y != 0) GetVelocity().y = jet.y * FIREBALL_JUMP_FORCE;
	}

	RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
	if (!collisionCal->AABB(cam, hitbox)) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}

	for each (shared_ptr<CollisionResult> coll in coResult)
	{
		if (MEntityType::IsMario(coll->Object->GetObjectType())) continue;
		if (coll->Object->GetObjectType() == MEntityType::VenusFireBall) continue;
		if (MEntityType::IsEnemy(coll->Object->GetObjectType())) {
			EffectServer::GetInstance()->SpawnEffect(make_shared<SmokeSpotFX>(Position + collisionCal->GetClampDistance()));
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
			continue;
		}
		if (!coll->Object->IsGetThrough(*this, coll->SAABBResult.Direction)) {
			if (!MEntityType::IsTile(coll->Object->GetObjectType()) || coll->SAABBResult.Direction != Direction::Top) {
				EffectServer::GetInstance()->SpawnEffect(make_shared<SmokeSpotFX>(Position + collisionCal->GetClampDistance()));
				SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());

			}
		}
	}
}

void MarioFireBall::FinalUpdate()
{
	GameObject::FinalUpdate();
	//collisionCal->Clear();
}

void MarioFireBall::Render()
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	this->animations["Default"]->GetTransform()->Position = GetPosition() - cam + size / 2;
	this->animations["Default"]->Render();
}

ObjectType MarioFireBall::GetObjectType()
{
	return MEntityType::MarioFireBall;
}

RectF MarioFireBall::GetHitBox()
{
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
	return hitbox;
}

bool MarioFireBall::IsGetThrough(GameObject& object, Direction direction)
{
	return false;
}

float MarioFireBall::GetDamageFor(GameObject& object, Direction direction)
{
	return 3.0f;
}
