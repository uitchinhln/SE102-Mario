#include "FlyingRedKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MarioGame.h"
#include "Mario.h"
#include "GameEvent.h"
#include "SceneManager.h"
#include "RedCrouchKoopas.h"
#include "EffectServer.h"
#include "KoopasDieFX.h"

FlyingRedKoopas::FlyingRedKoopas(shared_ptr<Koopas> koopas, Vec2 position, float range)
{
	this->koopas = koopas;
	this->rootPos = position;
	this->range = range;

	KP_SPEED = 0.12;
	size = Vec2(30, 45);

	koopas->GetLiveState() = KoopasLifeStates::ALIVE;

	DWORD dt = MarioGame::Time().ElapsedGameTime;
	koopas->SetFacing(1);

	koopas->GetGravity() = 0;
	koopas->SetVelocity(Vec2(0, KP_SPEED));

	koopas->GetDistance() = koopas->GetVelocity() * (float)dt;
}

void FlyingRedKoopas::InitResource(bool force)
{
    if (animations.size() < 1) {
        animations["Move"] = AnimationManager::GetInstance()->Get("ani-red-koopa-paratroopa-fly")->Clone();
    }
}

void FlyingRedKoopas::Update()
{
	DefaultKoopas::Update();

	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (abs(k->GetPosition().y - rootPos.y) > range) {
			if (k->GetPosition().y - rootPos.y < 0) {
				k->SetVelocity(Vec2(0, KP_SPEED));
			}
			else {
				k->SetVelocity(Vec2(0, -KP_SPEED));
			}
		}

		if (rootPos.x < MarioGame::GetInstance()->GetMario()->GetPosition().x) {
			k->SetFacing(-1);
		}
		else {
			k->SetFacing(-1);
		}
	}
}

void FlyingRedKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLifeStates::DIE;
			OnDeath(Vec2(0 * 0.1f, -0.6f));

			//Giet rua
			__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SOLID_BLOCK, k->GetPosition(), k->GetObjectType());
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = 0;
			if (jet.y != 0) k->GetVelocity().y = -k->GetVelocity().y;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						k->SetPower(make_shared<RedCrouchKoopas>(k));

						//Dap rua
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

void FlyingRedKoopas::OnDeath(Vec2 veloc)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<KoopasDieFX>(k->GetPosition(), veloc, "ani-red-koopa-troopa-crouch"));
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
	}
}

ObjectType FlyingRedKoopas::GetObjectType()
{
    return MEntityType::RedKoopasFlying;
}
