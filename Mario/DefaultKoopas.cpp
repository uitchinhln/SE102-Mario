#include "DefaultKoopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Koopas.h"
#include "SceneManager.h"

DefaultKoopas::DefaultKoopas(shared_ptr<Koopas> holder)
{
	this->holder = holder;
	
	this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-move")->Clone();
	this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();

	this->animations["Die"]->GetTransform()->Scale.y = -1;

	holder->GetDestroyTimer().Stop();

	holder->GetLiveState() = KoopasLiveStates::ALIVE;

	DWORD dt = CGame::Time().ElapsedGameTime;
	holder->SetFacing(-1);

	holder->GetGravity() = KP_GRAVITY;
	holder->SetVelocity(Vec2(KP_SPEED * holder->GetFacing(), 0));

	holder->GetDistance() = holder->GetVelocity() * dt;
}

void DefaultKoopas::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		collisionCal->CalcPotentialCollisions(coObj, false);
	}
}

void DefaultKoopas::StatusUpdate()
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::DIE) return;

		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();
		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() != 0)
		{
			Vec2 jet = collisionCal->GetJet();

			if (jet.x != 0) k->GetVelocity().x *= k->GetVelocity().x / abs(k->GetVelocity().x) * jet.x;
			if (jet.y != 0) k->GetVelocity().y = 0;

			for each (shared_ptr<CollisionResult> coll in coResult)
			{
				if (MEntityType::IsMario(coll->GameColliableObject->GetObjectType())) {
					if (coll->SAABBResult.Direction == Direction::Bottom) {
						
					}
				}

				if (MEntityType::IsMarioWeapon(coll->GameColliableObject->GetObjectType())) {
					k->GetLiveState() = KoopasLiveStates::DIE;
					k->SetVelocity(Vec2(jet.x * 0.1f, -0.6f));
					KP_DESTROY_DELAY = 3000;

					if (!k->GetDestroyTimer().IsRunning()) {
						k->GetDestroyTimer().Restart();
					}
				}
			}
		}

		Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
		if (k->GetPosition().x < 0.3 || k->GetPosition().y < 0.3 || k->GetPosition().x > mapBound.x || k->GetPosition().y > mapBound.y) {
			KP_DESTROY_DELAY = 100;
			if (!k->GetDestroyTimer().IsRunning()) {
				k->GetDestroyTimer().Restart();
			}
		}
	}
}

void DefaultKoopas::Update()
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		if (k->GetDestroyTimer().IsRunning() && k->GetDestroyTimer().Elapsed() >= KP_DESTROY_DELAY) {
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(k);
		}
		DWORD dt = CGame::Time().ElapsedGameTime;

		k->GetVelocity().y += k->GetGravity() * dt;
		k->GetDistance() = k->GetVelocity() * dt;
	}
}

void DefaultKoopas::FinalUpdate()
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		shared_ptr<CollisionCalculator> collisionCal = k->GetCollisionCalc();

		if (collisionCal && k->GetLiveState() == KoopasLiveStates::ALIVE) {
			k->GetDistance() = collisionCal->GetNewDistance();
		}
		k->GetPosition() += k->GetDistance();
		k->GetDistance() = k->GetVelocity() * CGame::Time().ElapsedGameTime;

		k->SetFacing(-k->GetVelocity().x / abs(k->GetVelocity().x));
	}
}

void DefaultKoopas::Render()
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		Animation ani = this->animations["Move"];

		if (k->GetLiveState() == KoopasLiveStates::DIE) {
			ani = this->animations["Die"];
		}

		ani->GetTransform()->Scale.x = k->GetFacing();
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
	if (shared_ptr<Koopas> k = holder.lock()) {
		return RectF(k->GetPosition().x, k->GetPosition().y, k->GetPosition().x + hitbox.x, k->GetPosition().y + hitbox.y);
	}
	return RectF(0, 0, 0, 0);
}

float DefaultKoopas::GetDamageFor(IColliable& object, Direction direction)
{
	if (shared_ptr<Koopas> k = holder.lock()) {
		if (k->GetLiveState() == KoopasLiveStates::ALIVE && MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
			return 1.0f;
		}
		return 0.0f;
	}
}
