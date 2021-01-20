#include "DefaultRedGoomba.h"
#include "MEntityType.h"
#include "RedGoomba.h"
#include "SceneManager.h"
#include "Game.h"
#include "EffectServer.h"
#include "RedGoombaDieFX.h"
#include "RedGoombaExplodeFX.h"
#include "ScoreFX.h"
#include "GameEvent.h"

DefaultRedGoomba::DefaultRedGoomba(shared_ptr<RedGoomba> holder)
{
	this->holder = holder;
	this->createTime = CGame::Time().TotalGameTime;

	holder->GetState() = RedGoombaState::WALK;

	DWORD dt = CGame::Time().ElapsedGameTime;
	//holder->SetFacing(-1);

	holder->GetGravity() = GB_GRAVITY;
	holder->GetVelocity() = Vec2(GB_SPEED * holder->GetFacing(), 0);

	holder->GetDistance() = holder->GetVelocity() * (float)dt;
}

void DefaultRedGoomba::InitResource()
{
	if (animations.size() < 1) {
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-red-goomba-walk")->Clone();
	}
}

void DefaultRedGoomba::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		if (g->GetState() != RedGoombaState::WALK) return;

		g->GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
	}
}

void DefaultRedGoomba::PositionUpdate()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		if (g->GetState() == RedGoombaState::WALK) {
			g->GetUpdatedDistance() = g->GetCollisionCalc()->GetClampDistance();
		}
		else {
			g->GetUpdatedDistance() = g->GetDistance();
		}
		g->GetPosition() += g->GetUpdatedDistance();
	}
}

void DefaultRedGoomba::StatusUpdate()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		if (g->GetState() != RedGoombaState::WALK) return;

		vector<shared_ptr<CollisionResult>> coResult = g->GetCollisionCalc()->GetLastResults();

		Vec2 velocity = g->GetVelocity();
		Vec2 position = g->GetPosition();
		Vec2 size = g->GetSize();
		RedGoombaState state = g->GetState();

		g->OnGround = false;
		if (coResult.size() > 0) {
			Vec2 jet = g->GetCollisionCalc()->GetJet();

			if (jet.x != 0) velocity.x = -velocity.x;
			if (jet.y != 0) velocity.y = 0;
			if (jet.y < 0) g->OnGround = true;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						state = RedGoombaState::DIE;

						position.y += size.y;
						size.y = 27;
						position.y -= size.y;

						EffectServer::GetInstance()->SpawnEffect(make_shared<RedGoombaDieFX>(position));
						SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(g);

						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO, position, g->GetObjectType());
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					float damage = coll->Object->GetDamageFor(*g, coll->SAABBResult.Direction);
					if (damage > 0) {
						state = RedGoombaState::EXPLODE;

						EffectServer::GetInstance()->SpawnEffect(make_shared<RedGoombaExplodeFX>(position, Vec2(jet.x * 0.1f, -0.65f)));
						SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(g);

						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO_WEAPON, position, g->GetObjectType());
					}
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (position.x < 0.3 - size.x || position.y < 0.3 - size.y || position.x > mapBound.x || position.y > mapBound.y) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(g);
		}

		g->GetVelocity() = velocity;
		g->GetPosition() = position;
		g->GetSize() = size;
		g->GetState() = state;
	}
}

void DefaultRedGoomba::Update()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		Vec2 velocity = g->GetVelocity();
		DWORD dt = CGame::Time().ElapsedGameTime;

		velocity.y += g->GetGravity() * (float)dt;
		g->GetDistance() = velocity * (float)dt;

		g->GetVelocity() = velocity;
	}
}

void DefaultRedGoomba::FinalUpdate()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		Vec2 velocity = g->GetVelocity();

		g->GetDistance() = velocity * (float)CGame::Time().ElapsedGameTime;
		g->SetFacing(velocity.x > 0 ? 1 : -1);
		g->GetCollisionCalc()->Clear();
	}
}

void DefaultRedGoomba::Render()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		InitResource();

		RedGoombaState state = g->GetState();
		Vec2 position = g->GetPosition();

		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation animation = this->animations["Walk"];

		animation->GetTransform()->Position = position - cam + g->GetSize() / 2;
		animation->Render();
	}
}

ObjectType DefaultRedGoomba::GetObjectType()
{
	return MEntityType::RedGoomba;
}

bool DefaultRedGoomba::IsGetThrough(GameObject& object, Direction direction)
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
	}
	return false;
}

float DefaultRedGoomba::GetDamageFor(GameObject& object, Direction direction)
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
	}
	return 0.0f;
}
