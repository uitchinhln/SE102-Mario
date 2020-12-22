#include "RedCrouchKoopas.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Mario.h"
#include "Game.h"
#include "RedMovingShell.h"
#include "DefRedKoopas.h"

RedCrouchKoopas::RedCrouchKoopas(shared_ptr<Koopas> koopas, bool flip) : CrouchKoopas(koopas, flip)
{
}

void RedCrouchKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-respawning")->Clone();
		this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-crouch")->Clone();

		if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}

		this->animations["Die"]->GetTransform()->Scale.y = -1;
	}
}

void RedCrouchKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();
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
					}
					break;
				}

				if (MEntityType::IsEnemy(coll->Object->GetObjectType())) {
					if (shared_ptr<Mario> m = k->GetHolder().lock()) {
						float damage = coll->Object->GetDamageFor(*k, coll->SAABBResult.Direction);
						if (damage > 0) {
							k->SetVelocity(Vec2(jet.x * 0.1f, -0.6f));
							KP_DESTROY_DELAY = 3000;

							if (!k->GetDestroyTimer().IsRunning()) {
								k->GetDestroyTimer().Restart();
								respawnTimer.Stop();
							}
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
						k->GetLiveState() = KoopasLiveStates::DIE;
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.6f));
						KP_DESTROY_DELAY = 3000;

						if (!k->GetDestroyTimer().IsRunning()) {
							k->GetDestroyTimer().Restart();
							respawnTimer.Stop();
						}
						break;
					}
					continue;
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 - size.x || k->GetPosition().y < 0.3 - size.y || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			KP_DESTROY_DELAY = 100;
			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
				respawnTimer.Stop();
			}
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

		if (k->GetDestroyTimer().IsRunning()) {
			k->GetLiveState() = KoopasLiveStates::DIE;
		}

		if (k->GetLiveState() == KoopasLiveStates::DIE) {
			if (shared_ptr<Mario> m = k->GetHolder().lock()) {
				m->ClearInhand();
			}
			k->ClearHolder();
		}
	}
}
