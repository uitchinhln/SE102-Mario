#include "MovingShell.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "CrouchKoopas.h"
#include "Game.h"
#include "QuestionBlock.h"
#include "ScoreFX.h"
#include "GameEvent.h"

MovingShell::MovingShell(shared_ptr<Koopas> koopas, bool flip) : DefaultKoopas()
{
	this->koopas = koopas;
	this->flip = flip;

	KP_SPEED *= 3;

	koopas->GetLiveState() = KoopasLifeStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(KP_SPEED * koopas->GetFacing(), 0));

	koopas->GetDistance() = koopas->GetVelocity() * (float)dt;
}

void MovingShell::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-shell-run")->Clone();

		if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}
	}
}

void MovingShell::FinalUpdate()
{
	DefaultKoopas::FinalUpdate();
}

void MovingShell::Update()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		DWORD dt = CGame::Time().ElapsedGameTime;

		k->GetVelocity().y += k->GetGravity() * (float)dt;
		k->GetDistance() = k->GetVelocity() * (float)dt;
	}
}

void MovingShell::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLifeStates::DIE;
			OnDeath(Vec2(0 * 0.1f, -0.6f));

			//Giet rua
			shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
			__raise (*GameEvent::GetInstance()).PlayerScoreEvent(__FILE__, effect, Score::S100);
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
						k->SetPower(make_shared<CrouchKoopas>(k, flip));

						//Dap rua
						shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
						__raise (*GameEvent::GetInstance()).PlayerScoreEvent(__FILE__, effect, Score::S100);
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
						__raise (*GameEvent::GetInstance()).PlayerScoreEvent(__FILE__, effect, Score::S100);
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

void MovingShell::Render()
{
	InitResource();
	if (shared_ptr<Koopas> k = koopas.lock()) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation ani = this->animations["Move"];

		ani->GetTransform()->Position = k->GetPosition() - cam + size / 2;
		ani->Render();
	}
}

ObjectType MovingShell::GetObjectType()
{
	return MEntityType::KoopasImposter;
}

float MovingShell::GetDamageFor(GameObject& object, Direction direction)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if ((k->GetLiveState() == KoopasLifeStates::ALIVE)
			&& MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
			return 1.0f;
		}
		if ((k->GetLiveState() == KoopasLifeStates::ALIVE)
			&& MEntityType::IsMarioWeapon(object.GetObjectType())) {
			return 1.0f;
		}
		if ((k->GetLiveState() == KoopasLifeStates::ALIVE)
			&& MEntityType::IsEnemy(object.GetObjectType())) {
			return 1.0f;
		}
	}
	return 0.0f;
}
