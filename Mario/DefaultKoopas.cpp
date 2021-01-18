#include "DefaultKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "SceneManager.h"
#include "CrouchKoopas.h"
#include "Game.h"
#include "EffectServer.h"
#include "KoopasDieFX.h"
#include "ScoreFX.h"
#include "GameEvent.h"

DefaultKoopas::DefaultKoopas()
{
}

DefaultKoopas::DefaultKoopas(shared_ptr<Koopas> koopas)
{
	this->koopas = koopas;

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
			if (jet.y != 0) k->GetVelocity().y = 0;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						k->SetPower(make_shared<CrouchKoopas>(k));

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

void DefaultKoopas::Update()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
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

		ani->GetTransform()->Scale.x = (float) -k->GetFacing();
		ani->GetTransform()->Position = k->GetPosition() - cam + size / 2;
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
		if (k->GetLiveState() == KoopasLifeStates::ALIVE && MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
			return 1.0f;
		}
		if (k->GetLiveState() == KoopasLifeStates::ALIVE  && MEntityType::IsMarioWeapon(object.GetObjectType())) {
			return 1.0f;
		}
	}
	return 0.0f;
}

void DefaultKoopas::OnDeath(Vec2 veloc)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<KoopasDieFX>(k->GetPosition(), veloc, "ani-green-koopa-troopa-crouch"));
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
	}
}

DefaultKoopas::~DefaultKoopas()
{
	DebugOut(L"Huy koopas power\n");
}
