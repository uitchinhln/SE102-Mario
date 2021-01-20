#include "JumpingRedGoomba.h"
#include "RedGoomba.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"
#include "RedGoombaExplodeFX.h"
#include "EffectServer.h"
#include "ScoreFX.h"
#include "GameEvent.h"
#include "MarioGame.h"

JumpingRedGoomba::JumpingRedGoomba(shared_ptr<RedGoomba> holder) : DefaultRedGoomba(holder)
{
	holder->GetGravity() = GB_GRAVITY;
	jumpStep = 0;
}

void JumpingRedGoomba::InitResource()
{
	if (animations.size() < 1) {
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-red-para-goomba-walk")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-red-para-goomba-fly")->Clone();
	}
}

void JumpingRedGoomba::Update()
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		Vec2 velocity = g->GetVelocity();

		if (g->OnGround)
		{
			int direction = 1;
			shared_ptr<Mario> player = MarioGame::GetInstance()->GetMario();

			switch (jumpStep)
			{
			case 0:
				direction = player->GetPosition().x < g->GetPosition().x ? -1 : 1;
				g->SetFacing(direction);
				velocity.x = direction * abs(velocity.x);
				jumpTimer.Restart();
				jumpStep = 1;
				break;
			case 1:
				if (jumpTimer.Elapsed() >= RED_PARAGOOMBA_BREAK_TIME)
				{
					jumpTimer.Restart();
					jumpStep = 2;
				}
				break;
			case 2:
				velocity.y = -RED_PARAGOOMBA_LOW_JUMP;
				g->OnGround = false;
				jumpStep = 3;
				break;
			case 3:
				velocity.y = -RED_PARAGOOMBA_HIGH_JUMP;
				g->OnGround = false;
				jumpStep = 4;
				break;
			case 4:
				jumpStep = 0;
				break;
			}
		}

		DWORD dt = CGame::Time().ElapsedGameTime;

		velocity.y += g->GetGravity() * (float)dt;
		g->GetDistance() = velocity * (float)dt;

		g->GetVelocity() = velocity;
	}
}

void JumpingRedGoomba::StatusUpdate()
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
						g->SetObjectState(make_shared<DefaultRedGoomba>(g));

						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO, position, g->GetObjectType());
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					float damage = coll->Object->GetDamageFor(*g, coll->SAABBResult.Direction);
					if (damage > 0) {
						state = RedGoombaState::EXPLODE;

						EffectServer::GetInstance()->SpawnEffect(make_shared<RedGoombaExplodeFX>(position, Vec2(jet.x * 0.1f, -0.65f)));
						SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(g);

						if (coll->Object->GetObjectType() == MEntityType::VoidBlock) {
							__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SPACE, g->GetPosition(), g->GetObjectType());
						}
						else {
							__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO_WEAPON, g->GetPosition(), g->GetObjectType());
						}
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

void JumpingRedGoomba::Render(D3DCOLOR overlay)
{
	if (shared_ptr<RedGoomba> g = holder.lock()) {
		InitResource();

		RedGoombaState state = g->GetState();
		Vec2 position = g->GetPosition();

		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation animation = jumpStep >= 2 ? this->animations["Fly"] : this->animations["Walk"];

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

		animation->GetTransform()->Position = position - cam + g->GetSize() / 2;
		animation->Render(overlay);
	}
}
