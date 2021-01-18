#include "JumpingKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "Game.h"
#include "SceneManager.h"
#include "CrouchKoopas.h"
#include "IEffect.h"
#include "ScoreFX.h"
#include "GameEvent.h"

JumpingKoopas::JumpingKoopas(shared_ptr<Koopas> koopas) : DefaultKoopas()
{
	this->koopas = koopas; 
	this->KP_SPEED = 0.12f;

	koopas->GetLiveState() = KoopasLifeStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;

	koopas->GetGravity() = KP_GRAVITY * 0.7;
	koopas->SetVelocity(Vec2(KP_SPEED * koopas->GetFacing(), 0));

	koopas->GetDistance() = koopas->GetVelocity() * (float)dt;
}

void JumpingKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-paratroopa-fly")->Clone();
	}
}

void JumpingKoopas::FinalUpdate()
{
	DefaultKoopas::FinalUpdate();
}

void JumpingKoopas::Update()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		DWORD dt = CGame::Time().ElapsedGameTime;

		k->GetVelocity().y += k->GetGravity() * (float)dt;
		k->GetDistance() = k->GetVelocity() * (float)dt;
	}
}

void JumpingKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLifeStates::DIE;
			OnDeath(Vec2(0 * 0.1f, -0.6f));

			//Giet rua
			shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
			__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S100);
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = -k->GetVelocity().x;
			if (jet.y != 0) {
				if (jet.y > 0) { 
					k->GetVelocity().y = 0; 
				}
				else {
					k->GetVelocity().y = jet.y * KOOPAS_JUMP_FORCE;
				}
			}

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						k->SetPower(make_shared<DefaultKoopas>(k));

						//Dap rua
						shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
						__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S100);
						return;
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
						k->SetPower(make_shared<CrouchKoopas>(k, true));
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.95f));
						return;
					}
					float damage = coll->Object->GetDamageFor(*k, coll->SAABBResult.Direction);
					if (damage > 0) {
						k->GetLiveState() = KoopasLifeStates::DIE;
						OnDeath(Vec2(jet.x * 0.1f, -0.6f));

						//Giet rua
						shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
						__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S100);
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

void JumpingKoopas::Render()
{
	InitResource();
	if (shared_ptr<Koopas> k = koopas.lock()) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation ani = this->animations["Move"];

		ani->GetTransform()->Scale.x = (float)-k->GetFacing();
		ani->GetTransform()->Position = k->GetPosition() - cam + size / 2;
		ani->Render();
	}
}

ObjectType JumpingKoopas::GetObjectType()
{
	return MEntityType::KoopasJumping;
}
