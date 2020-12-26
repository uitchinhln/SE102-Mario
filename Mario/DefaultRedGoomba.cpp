#include "DefaultRedGoomba.h"
#include "MEntityType.h"
#include "RedGoomba.h"
#include "SceneManager.h"
#include "Game.h"

DefaultRedGoomba::DefaultRedGoomba(shared_ptr<RedGoomba> holder)
{
	this->holder = holder;

	holder->GetDestroyTimer().Stop();
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
		this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-red-goomba-die")->Clone();
		this->animations["Explode"] = AnimationManager::GetInstance()->Get("ani-red-goomba-idle")->Clone();

		this->animations["Explode"]->GetTransform()->Scale.y = -1;
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
		Stopwatch destroyTimer = g->GetDestroyTimer();
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
						velocity = VECTOR_0;
						g->GetGravity() = 0;

						position.y += size.y;
						size.y = 27;
						position.y -= size.y;

						if (!destroyTimer.IsRunning()) {
							destroyTimer.Restart();
						}
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					float damage = coll->Object->GetDamageFor(*g, coll->SAABBResult.Direction);
					if (damage > 0) {
						state = RedGoombaState::EXPLODE;
						velocity = Vec2(jet.x * 0.1f, -0.65f);
						GB_DESTROY_DELAY = 3000;

						if (!destroyTimer.IsRunning()) {
							destroyTimer.Restart();
						}
					}
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (position.x < 0.3 - size.x || position.y < 0.3 - size.y || position.x > mapBound.x || position.y > mapBound.y) {
			GB_DESTROY_DELAY = 100;
			if (!destroyTimer.IsRunning()) {
				destroyTimer.Restart();
			}
		}

		g->GetVelocity() = velocity;
		g->GetPosition() = position;
		g->GetSize() = size;
		g->GetDestroyTimer() = destroyTimer;
		g->GetState() = state;
	}
}

void DefaultRedGoomba::Update()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		Vec2 velocity = g->GetVelocity();
		Stopwatch destroyTimer = g->GetDestroyTimer();

		if (destroyTimer.IsRunning() && destroyTimer.Elapsed() >= GB_DESTROY_DELAY) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(g);
		}
		DWORD dt = CGame::Time().ElapsedGameTime;

		velocity.y += g->GetGravity() * (float)dt;
		g->GetDistance() = velocity * (float)dt;

		g->GetVelocity() = velocity;
		g->GetDestroyTimer() = destroyTimer;
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

		switch (state)
		{
		case RedGoombaState::DIE:
			animation = this->animations["Die"];
			break;
		case RedGoombaState::EXPLODE:
			animation = this->animations["Explode"];
			break;
		default:
			break;
		}

		animation->GetTransform()->Position = position - cam;
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
