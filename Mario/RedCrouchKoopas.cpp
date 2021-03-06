#include "RedCrouchKoopas.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"
#include "RedMovingShell.h"
#include "DefRedKoopas.h"
#include "EffectServer.h"
#include "KoopasDieFX.h"
#include "ScoreFX.h"
#include "GameEvent.h"
#include "MarioGame.h"

RedCrouchKoopas::RedCrouchKoopas(shared_ptr<Koopas> koopas, bool flip) : CrouchKoopas(koopas, flip)
{
}

void RedCrouchKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-respawning")->Clone();

		if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}
	}
}

void RedCrouchKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			if (!k->GetHolder().lock()) {
				k->GetLiveState() = KoopasLifeStates::DIE;
				OnDeath(Vec2(0 * 0.1f, -0.6f));

				__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SOLID_BLOCK, k->GetPosition(), k->GetObjectType());
			}
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = 0;
			if (jet.y != 0) k->GetVelocity().y = 0;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsTile(coll->Object->GetObjectType())) {
					k->GetVelocity().x = 0;
					continue;
				}

				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (k->GetObjectType() == MEntityType::KoopasCrouch) {
						shared_ptr<Mario> m = dynamic_pointer_cast<Mario>(coll->Object);
						k->SetFacing(m->GetFacing());
						k->SetPower(make_shared<RedMovingShell>(k, flip));

						if (coll->SAABBResult.Direction == Direction::Bottom && CGame::Time().TotalGameTime - createTime < 1000) {
							//Dap rua
							__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO, k->GetPosition(), k->GetObjectType());
						}
					}
					break;
				}

				if (MEntityType::IsEnemy(coll->Object->GetObjectType())) {
					if (shared_ptr<Mario> m = k->GetHolder().lock()) {
						float damage = coll->Object->GetDamageFor(*k, coll->SAABBResult.Direction);
						if (damage > 0) {
							OnDeath(Vec2(jet.x * 0.1f, -0.6f));
							break;
						}
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.95f));
						this->flip = true;
						respawnTimer.Restart();
						continue;
					}
					float damage = coll->Object->GetDamageFor(*k, coll->SAABBResult.Direction);
					if (damage > 0) {
						k->GetLiveState() = KoopasLifeStates::DIE;
						OnDeath(Vec2(jet.x * 0.1f, -0.6f));

						if (coll->Object->GetObjectType() == MEntityType::VoidBlock) {
							__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SPACE, k->GetPosition(), k->GetObjectType());
						}
						else {
							__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO_WEAPON, k->GetPosition(), k->GetObjectType());
						}
						break;
					}
					continue;
				}

				if (coll->Object->GetObjectType() == MEntityType::Brick && coll->SAABBResult.Direction == Direction::Top) {
					shared_ptr<Mario> mario = MarioGame::GetInstance()->GetMario();
					if (mario->GetObjectType() != MEntityType::SmallMario && coll->Object->HasCollideWith(mario->GetID())) {
						shared_ptr<CollisionResult> _rs = coll->Object->GetCollisionCalc()->Get(mario->GetID());
						if (_rs->SAABBResult.Direction == Direction::Top) {
							k->SetVelocity(Vec2(jet.x * 0.1f, -0.65f));
						}
					}
				}

				if (coll->Object->GetObjectType() == MEntityType::QuestionBlock && coll->SAABBResult.Direction == Direction::Top) {
					shared_ptr<Mario> mario = MarioGame::GetInstance()->GetMario();
					if (coll->Object->GetCollisionCalc()->Has(mario->GetID())) {
						shared_ptr<CollisionResult> _rs = coll->Object->GetCollisionCalc()->Get(mario->GetID());
						if (_rs->SAABBResult.Direction == Direction::Top) {
							k->SetVelocity(Vec2(jet.x * 0.1f, -0.65f));
						}
					}
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 - size.x || k->GetPosition().y < 0.3 - size.y || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
		}
	}
}

void RedCrouchKoopas::FinalUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		k->GetDistance() = k->GetVelocity() * (float)CGame::Time().ElapsedGameTime;

		if (respawnTimer.Elapsed() >= KP_RESPAWN_TIME) {
			k->SetPower(make_shared<DefRedKoopas>(k));

			if (shared_ptr<Mario> m = k->GetHolder().lock()) {
				m->ClearInhand();
				k->ClearHolder();
			}
		}

		//if (k->GetDestroyTimer().IsRunning()) {
		//	k->GetLiveState() = KoopasLifeStates::DIE;
		//}

		if (k->GetLiveState() == KoopasLifeStates::DIE) {
			if (shared_ptr<Mario> m = k->GetHolder().lock()) {
				m->ClearInhand();
			}
			k->ClearHolder();
		}
	}
}

void RedCrouchKoopas::OnDeath(Vec2 veloc)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<KoopasDieFX>(k->GetPosition(), veloc, "ani-red-koopa-troopa-crouch"));
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
	}
}
