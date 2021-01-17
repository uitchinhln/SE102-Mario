#include "DefaultKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "SceneManager.h"
#include "CrouchKoopas.h"
#include "Game.h"

DefaultKoopas::DefaultKoopas()
{
}

DefaultKoopas::DefaultKoopas(shared_ptr<Koopas> koopas)
{
	this->koopas = koopas;

	koopas->GetDestroyTimer().Stop();

	koopas->GetLiveState() = KoopasLifeStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;
	koopas->SetFacing(-1);

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(KP_SPEED * koopas->GetFacing(), 0));

	koopas->GetDistance() = koopas->GetVelocity() * (float) dt;
}

void DefaultKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-move")->Clone();
		this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();

		this->animations["Die"]->GetTransform()->Scale.y = -1;
	}
}

void DefaultKoopas::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		k->GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
	}
}

void DefaultKoopas::PositionUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::ALIVE) {
			shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();
			k->GetUpdatedDistance() = collisionCal->GetClampDistance();
		}
		else {
			k->GetUpdatedDistance() = k->GetDistance();
		}
		k->GetPosition() += k->GetUpdatedDistance();
	}
}

void DefaultKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLifeStates::DIE;
			k->SetVelocity(Vec2(0 * 0.1f, -0.6f));
			KP_DESTROY_DELAY = 3000;

			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
			}
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = -k->GetVelocity().x;
			if (jet.y != 0) k->GetVelocity().y = 0;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						k->SetPower(make_shared<CrouchKoopas>(k));
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
						k->SetVelocity(Vec2(jet.x * 0.1f, -0.6f));
						KP_DESTROY_DELAY = 3000;

						if (!k->GetDestroyTimer().IsRunning()) {
							k->GetDestroyTimer().Restart();
						}
					}					
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 - size.x || k->GetPosition().y < 0.3 - size.y || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			KP_DESTROY_DELAY = 100;
			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
			}
		}
	}
}

void DefaultKoopas::Update()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetDestroyTimer().IsRunning() && k->GetDestroyTimer().Elapsed() >= KP_DESTROY_DELAY) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
		}
		DWORD dt = CGame::Time().ElapsedGameTime;

		k->GetVelocity().y += k->GetGravity() * (float)dt;
		k->GetDistance() = k->GetVelocity() * (float) dt;
	}
}

void DefaultKoopas::FinalUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		k->GetDistance() = k->GetVelocity() * (float) CGame::Time().ElapsedGameTime;
		k->SetFacing(k->GetVelocity().x > 0 ? 1 : -1);
	}
}

void DefaultKoopas::Render()
{
	InitResource();

	if (shared_ptr<Koopas> k = koopas.lock()) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation ani = this->animations["Move"];

		if (k->GetLiveState() == KoopasLifeStates::DIE) {
			ani = this->animations["Die"];
		}

		ani->GetTransform()->Scale.x = (float) -k->GetFacing();
		ani->GetTransform()->Position = k->GetPosition() - cam;
		ani->Render();
	}
}

ObjectType DefaultKoopas::GetObjectType()
{
	return MEntityType::Koopas;
}

RectF DefaultKoopas::GetHitBox()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		hitbox.left = k->GetPosition().x;
		hitbox.top = k->GetPosition().y;
		hitbox.right = k->GetPosition().x + size.x;
		hitbox.bottom = k->GetPosition().y + size.y;
		return hitbox;
	}
	return RectF(0, 0, 0, 0);
}

float DefaultKoopas::GetDamageFor(GameObject& object, Direction direction)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if ((k->GetLiveState() == KoopasLifeStates::ALIVE || k->GetDestroyTimer().Elapsed() <= 5)
			&& MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
			return 1.0f;
		}
		if ((k->GetLiveState() == KoopasLifeStates::ALIVE || k->GetDestroyTimer().Elapsed() <= 5) 
			&& MEntityType::IsMarioWeapon(object.GetObjectType())) {
			return 1.0f;
		}
	}
	return 0.0f;
}

DefaultKoopas::~DefaultKoopas()
{
	//DebugOut(L"Huy koopas power\n");
}
