#include "DefRedKoopas.h"
#include "DefaultKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "SceneManager.h"
#include "Game.h"


DefRedKoopas::DefRedKoopas(shared_ptr<Koopas> koopas)
{
	this->koopas = koopas;

	koopas->GetDestroyTimer().Stop();

	koopas->GetLiveState() = KoopasLiveStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;
	koopas->SetFacing(1);

	koopas->GetGravity() = KP_GRAVITY;
	koopas->SetVelocity(Vec2(KP_SPEED * koopas->GetFacing(), 0));

	koopas->GetDistance() = koopas->GetVelocity() * (float)dt;
}

void DefRedKoopas::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-move")->Clone();
		this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-red-koopa-troopa-crouch")->Clone();

		this->animations["Die"]->GetTransform()->Scale.y = -1;
	}
}

void DefRedKoopas::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::DIE) return;

		if (k->OnGround) {
			vector<shared_ptr<GameObject>> rcResults;

			Direction direction = Direction::Left;
			Vec2 startPoint = k->GetPosition();
			startPoint.y += size.y + 0.2;
			startPoint.x += size.x;

			if (k->GetFacing() > 0) {
				direction = Direction::Right;
				startPoint.x -= size.x;
			}

			raycaster.SetInput(coObj);
			raycaster.Shoot(startPoint, direction, size.y + size.y + 1, rcResults);

			rcResults.erase(remove_if(rcResults.begin(), rcResults.end(), [k](const shared_ptr<GameObject>& obj) {
				return !MEntityType::IsTile(obj->GetObjectType()) || obj->GetID() == k->GetID();
				}), rcResults.end());

			if (rcResults.size() > 0) {
				vector<shared_ptr<Vec2>> edges;
				raycaster.MergeBox(rcResults, direction, edges);

				RectF hitbox = k->GetHitBox();

				if (direction == Direction::Left) {
					for each (shared_ptr<Vec2> box in edges)
					{
						if (hitbox.left <= box->x && hitbox.right > box->x) {
							DWORD dt = CGame::Time().ElapsedGameTime;
							k->GetVelocity().x = -k->GetVelocity().x;
							k->GetDistance() = k->GetVelocity() * (float)dt;
							break;
						}
					}
				}
				else {
					for each (shared_ptr<Vec2> box in edges)
					{
						if (hitbox.left < box->y && hitbox.right >= box->y) {
							DWORD dt = CGame::Time().ElapsedGameTime;
							k->GetVelocity().x = -k->GetVelocity().x;
							k->GetDistance() = k->GetVelocity() * (float)dt;
							break;
						}
					}
				}
			}	
		}

		k->GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
	}
}

void DefRedKoopas::PositionUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::ALIVE) {
			shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();
			k->GetUpdatedDistance() = collisionCal->GetClampDistance();
		}
		else {
			k->GetUpdatedDistance() = k->GetDistance();
		}
		k->GetPosition() += k->GetUpdatedDistance();
	}
}

void DefRedKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = koopas.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal->HasOverlapped()) {
			k->GetLiveState() = KoopasLiveStates::DIE;
			k->SetVelocity(Vec2(0 * 0.1f, -0.6f));
			KP_DESTROY_DELAY = 3000;

			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
			}
			return;
		}

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		k->OnGround = false;
		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x = -k->GetVelocity().x;
			if (jet.y != 0) k->GetVelocity().y = 0;
			if (jet.y <  0) k->OnGround = true;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->Object->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						//k->SetPower(make_shared<CrouchKoopas>(k));
						return;
					}
				}

				if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
					if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
						//k->SetPower(make_shared<CrouchKoopas>(k, true));
						//k->SetVelocity(Vec2(jet.x * 0.1f, -0.95f));
						return;
					}
					float damage = coll->Object->GetDamageFor(*k, coll->SAABBResult.Direction);
					if (damage > 0) {
						k->GetLiveState() = KoopasLiveStates::DIE;
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

ObjectType DefRedKoopas::GetObjectType()
{
	return MEntityType::RedKoopas;
}

DefRedKoopas::~DefRedKoopas()
{
}