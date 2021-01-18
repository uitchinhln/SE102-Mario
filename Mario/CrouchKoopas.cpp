#include "CrouchKoopas.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MovingShell.h"
#include "Game.h"
#include "ScoreFX.h"
#include "GameEvent.h"

CrouchKoopas::CrouchKoopas(shared_ptr<Koopas> koopas, bool flip) : DefaultKoopas()
{
	this->koopas = koopas;
	this->flip = flip;

	koopas->GetLiveState() = KoopasLifeStates::ALIVE;

	createTime = CGame::Time().TotalGameTime;
	DWORD dt = CGame::Time().ElapsedGameTime;

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(0, 0));

	koopas->GetDistance() = koopas->GetVelocity() * (float) dt;

	koopas->SetRenderOrder(125);

	respawnTimer.Start();
}

void CrouchKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
		this->animations["Respawn"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-respawning")->Clone();

		if (this->flip) {
			this->animations["Move"]->GetTransform()->Scale.y = -1;
		}
	}
}

void CrouchKoopas::FinalUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		k->GetDistance() = k->GetVelocity() * (float) CGame::Time().ElapsedGameTime;

		if (respawnTimer.Elapsed() >= KP_RESPAWN_TIME) {
			k->SetPower(make_shared<DefaultKoopas>(k));

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

void CrouchKoopas::HeldUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (shared_ptr<Mario> m = k->GetHolder().lock()) {
			if (k->GetLiveState() == KoopasLifeStates::DIE) {
				m->ClearInhand();
				k->ClearHolder();
				return;
			}

			k->SetVelocity(Vec2(m->GetVelocity().x, m->GetVelocity().y));
			k->GetDistance() = Vec2(m->GetDistance().x, m->GetDistance().y);
			if (m->GetFacing() > 0) {
				k->SetPosition(Vec2(
					m->GetHitBox().right - (m->GetObjectType() != MEntityType::SmallMario ? 12 : 4),
					m->GetHitBox().bottom - size.y - (m->GetObjectType() != MEntityType::SmallMario ? 9 : 2)));
			}
			else {
				k->SetPosition(Vec2(
					m->GetHitBox().left - size.x + (m->GetObjectType() != MEntityType::SmallMario ? 12 : 4),
					m->GetHitBox().bottom - size.y - (m->GetObjectType() != MEntityType::SmallMario ? 9 : 2)));
			}
			if (m->GetWarpState() == WarpStates::VERTICAL) {
				k->SetPosition(Vec2(
					m->GetHitBox().left,
					k->GetPosition().y
				));
			}
		}
	}
}

void CrouchKoopas::Update()
{
	DefaultKoopas::Update();
	HeldUpdate();
}

void CrouchKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			if (!k->GetHolder().lock()) {
				k->GetLiveState() = KoopasLifeStates::DIE;
				OnDeath(Vec2(0 * 0.1f, -0.6f));

				//Giet rua
				shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
				__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S100);
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
						k->SetPower(make_shared<MovingShell>(k, flip));

						if (coll->SAABBResult.Direction == Direction::Bottom && CGame::Time().TotalGameTime - createTime < 1000) {
							//Dap rua
							shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S200);
							__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S200);
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

						//Giet rua
						shared_ptr<IEffect> effect = make_shared<ScoreFX>(k->GetPosition(), Score::S100);
						__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S100);
						break;
					}
					continue;
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 - size.x || k->GetPosition().y < 0.3 - size.y || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
		}
	}
}

void CrouchKoopas::Render()
{
	InitResource();
	if (respawnTimer.IsRunning() && KP_RESPAWN_TIME - respawnTimer.Elapsed() <= 3000) {
		if (shared_ptr<Koopas> k = koopas.lock()) {
			Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

			Animation ani = this->animations["Respawn"];

			if (flip)
				ani->GetTransform()->Scale.y = -1;

			ani->GetTransform()->Position = k->GetPosition() - cam + size / 2;
			ani->Render();
		}
		return;
	}
	if (this->flip) {
		this->animations["Move"]->GetTransform()->Scale.y = -1;
	}
	DefaultKoopas::Render();
}

ObjectType CrouchKoopas::GetObjectType()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetHolder().lock()) {
			return MEntityType::KoopasPassenger;
		}
	}
	return MEntityType::KoopasCrouch;
}

float CrouchKoopas::GetDamageFor(GameObject& object, Direction direction)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLifeStates::DIE) return 0.0f;

		if (MEntityType::IsEnemy(object.GetObjectType()) && k->GetHolder().lock()) {
			return 999.0f;
		}
		if (MEntityType::IsMarioWeapon(object.GetObjectType())) {
			return 1.0f;
		}
	}
	return 0.0f;
}

CrouchKoopas::~CrouchKoopas()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		k->SetRenderOrder(1000);
	}
}
