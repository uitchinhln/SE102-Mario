#include "RedMovingShell.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Game.h"
#include "QuestionBlock.h"
#include "RedCrouchKoopas.h"
#include "KoopasDieFX.h"
#include "EffectServer.h"
#include "MarioUtils.h"
#include "ScoreFX.h"
#include "GameEvent.h"

RedMovingShell::RedMovingShell(shared_ptr<Koopas> koopas, bool flip) : MovingShell(koopas, flip)
{
}

void RedMovingShell::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-shell-run")->Clone();

		if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}
	}
}

void RedMovingShell::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLifeStates::DIE;
			OnDeath(Vec2(0 * 0.1f, -0.6f));

			if (k->GetHolder().lock()) {
				//Giet rua
				__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SOLID_BLOCK, k->GetPosition(), k->GetObjectType());
			}
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = -k->GetVelocity().x;
			if (jet.y != 0) k->GetVelocity().y = 0;

			bool hitQBlock = false;
			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (!hitQBlock && coll->Object->GetObjectType() == MEntityType::QuestionBlock && ToVector(coll->SAABBResult.Direction).x != 0) {
					shared_ptr<QuestionBlock> block = dynamic_pointer_cast<QuestionBlock>(coll->Object);
					if (block->GetState() == QuestionBlockStates::Available) {
						hitQBlock = true;
						block->Hit();
					}
					continue;
				}

				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						k->SetPower(make_shared<RedCrouchKoopas>(k, flip));

						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO, k->GetPosition(), k->GetObjectType());
						return;
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
						k->SetPower(make_shared<RedCrouchKoopas>(k, true));
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.95f));
						return;
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

void RedMovingShell::OnDeath(Vec2 veloc)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<KoopasDieFX>(k->GetPosition(), veloc, "ani-red-koopa-troopa-crouch"));
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
	}
}
