#include "CrouchKoopas.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Mario.h"

CrouchKoopas::CrouchKoopas(shared_ptr<Koopas> koopas, bool flip) : DefaultKoopas()
{
	this->koopas = koopas;

	this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
	this->animations["Respawn"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-respawning")->Clone();
	this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();

	if (this->flip = flip) {
		this->animations["Move"]->GetTransform()->Scale.y = -1;
	}

	this->animations["Die"]->GetTransform()->Scale.y = -1;

	koopas->GetDestroyTimer().Stop();

	koopas->GetLiveState() = KoopasLiveStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;
	koopas->SetFacing(1);

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(0, 0));

	koopas->GetDistance() = koopas->GetVelocity() * dt;

	respawnTimer.Start();
}

void CrouchKoopas::FinalUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal && k->GetLiveState() == KoopasLiveStates::ALIVE) {
			k->GetDistance() = collisionCal->GetNewDistance();
			//k->GetDistance().x = 0;
		}
		k->GetPosition() += k->GetDistance();
		k->GetDistance() = k->GetVelocity() * CGame::Time().ElapsedGameTime;

		if (respawnTimer.Elapsed() >= KP_RESPAWN_TIME) {
			k->SetPower(make_shared<DefaultKoopas>(k));

			if (shared_ptr<Mario> m = k->GetHolder().lock()) {
				m->ClearInhand();
				k->ClearHolder();
			}
		}
	}
}

void CrouchKoopas::HeldUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (shared_ptr<Mario> m = k->GetHolder().lock()) {
			k->GetGravity() = 0;
			k->SetVelocity(Vec2(m->GetVelocity().x, m->GetVelocity().y));
			k->GetDistance() = Vec2(m->GetDistance().x, m->GetDistance().y);
			if (m->GetFacing() > 0) {
				k->SetPosition(Vec2(
					m->GetHitBox().right - (m->GetObjectType() != MEntityType::SmallMario ? 12 : 4),
					m->GetHitBox().bottom - hitbox.y - (m->GetObjectType() != MEntityType::SmallMario ? 9 : 2)));
			}
			else {
				k->SetPosition(Vec2(
					m->GetHitBox().left - hitbox.x + (m->GetObjectType() != MEntityType::SmallMario ? 12 : 4),
					m->GetHitBox().bottom - hitbox.y - (m->GetObjectType() != MEntityType::SmallMario ? 9 : 2)));
			}
		}
	}
}

void CrouchKoopas::Update()
{
	HeldUpdate();
	DefaultKoopas::Update();
}

void CrouchKoopas::StatusUpdate()
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
				if (MEntityType::IsTile(coll->GameColliableObject->GetObjectType())) {
					k->GetVelocity().x = 0;
					continue;
				}

				if (MEntityType::IsMario(coll->GameColliableObject->GetObjectType())) {
					
					break;
				}

				if (MEntityType::IsMarioWeapon(coll->GameColliableObject->GetObjectType())) {
					if (coll->GameColliableObject->GetObjectType() == MEntityType::MarioTailed) {
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.95f));
						this->flip = true;
						this->animations["Move"]->GetTransform()->Scale.y = -1;
						respawnTimer.Restart();
						continue;
					}
					k->GetLiveState() = KoopasLiveStates::DIE;
					k->SetVelocity(Vec2(jet.x * 0.1f, -0.6f));
					KP_DESTROY_DELAY = 3000;

					if (!k->GetDestroyTimer().IsRunning()) {
						k->GetDestroyTimer().Restart();
						respawnTimer.Stop();
					}
					continue;
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 || k->GetPosition().y < 0.3 || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			KP_DESTROY_DELAY = 100;
			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
				respawnTimer.Stop();
			}
		}
	}
}

void CrouchKoopas::Render()
{
	if (respawnTimer.IsRunning() && KP_RESPAWN_TIME - respawnTimer.Elapsed() <= 3000) {
		if (shared_ptr<Koopas> k = koopas.lock()) {
			Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

			Animation ani = this->animations["Respawn"];

			if (flip)
				ani->GetTransform()->Scale.y = -1;

			ani->GetTransform()->Position = k->GetPosition() - cam;
			ani->Render();
		}
		return;
	}
	DefaultKoopas::Render();
}

ObjectType CrouchKoopas::GetObjectType()
{
	return MEntityType::KoopasCrouch;
}

float CrouchKoopas::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}