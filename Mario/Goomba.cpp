#include "Goomba.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"

Goomba::Goomba()
{
	this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-goomba-walk")->Clone();
	this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-goomba-die")->Clone();
	this->animations["Explode"] = AnimationManager::GetInstance()->Get("ani-goomba-idle")->Clone();

	this->animations["Explode"]->GetTransform()->Scale.y = -1;

	destroyTimer.Stop();
	state = GoombaState::WALK;

	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = GB_GRAVITY;
	this->Velocity = Vec2(GB_SPEED * facing, 0);

	this->Distance = Velocity * dt;
}

void Goomba::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	if (state != GoombaState::WALK) return;

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	collisionCal->CalcPotentialCollisions(coObj, false);
}

void Goomba::StatusUpdate()
{
	if (state != GoombaState::WALK) return;

	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() == 0 || state != GoombaState::WALK)
	{
		//GetPosition() += GetDistance();
	}
	else {
		//GetDistance() = collisionCal->GetNewDistance();
		Vec2 jet = collisionCal->GetJet();
		//GetPosition() += GetDistance();

		if (jet.x != 0) GetVelocity().x *= GetVelocity().x / abs(GetVelocity().x) * jet.x;
		if (jet.y != 0) GetVelocity().y = 0;

		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMario(coll->GameColliableObject->GetObjectType())) {
				if (coll->SAABBResult.Direction == Direction::Bottom) {
					state = GoombaState::DIE;
					SetVelocity(VECTOR_0);
					Gravity = 0;

					Position.y += hitbox.y;
					hitbox.y = 27;
					Position.y -= hitbox.y;

					if (!destroyTimer.IsRunning()) {
						destroyTimer.Restart();
					}
				}
			}

			if (MEntityType::IsMarioWeapon(coll->GameColliableObject->GetObjectType())) {
				float damage = coll->GameColliableObject->GetDamageFor(*this, coll->SAABBResult.Direction);
				if (damage > 0) {
					state = GoombaState::EXPLODE;
					Velocity = Vec2(jet.x * 0.1f, -0.65f);
					GB_DESTROY_DELAY = 3000;

					if (!destroyTimer.IsRunning()) {
						destroyTimer.Restart();
					}
				}
			}
		}
	}

	Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
	if (GetPosition().x < 0.3 || GetPosition().y < 0.3 || GetPosition().x > mapBound.x || GetPosition().y > mapBound.y) {
		GB_DESTROY_DELAY = 100;
		if (!destroyTimer.IsRunning()) {
			destroyTimer.Restart();
		}
	}
}

void Goomba::Update()
{
	if (destroyTimer.IsRunning() && destroyTimer.Elapsed() >= GB_DESTROY_DELAY) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}
	DWORD dt = CGame::Time().ElapsedGameTime;

	GetVelocity().y += GetGravity() * dt;
	GetDistance() = GetVelocity() * dt;
}

void Goomba::FinalUpdate()
{
	if (collisionCal && state == GoombaState::WALK) {
		Distance = collisionCal->GetNewDistance();
	}
	Position += Distance;
	Distance = Velocity * CGame::Time().ElapsedGameTime;
}

void Goomba::Render()
{
	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
	
	Animation animation = this->animations["Walk"];

	switch (state)
	{
	case GoombaState::DIE:
		animation = this->animations["Die"];
		break;
	case GoombaState::EXPLODE:
		animation = this->animations["Explode"];
		break;
	default:
		break;
	}

	animation->GetTransform()->Position = GetPosition() - cam;
	animation->Render();
}

ObjectType Goomba::GetObjectType()
{
	return MEntityType::Goomba;
}

RectF Goomba::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + hitbox.x, Position.y + hitbox.y);
}

bool Goomba::IsGetThrough(IColliable& object, Direction direction)
{
	return state != GoombaState::WALK;
}

float Goomba::GetDamageFor(IColliable& object, Direction direction)
{
	if ((state == GoombaState::WALK || destroyTimer.Elapsed() <= 5) && MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
		return 1.0f;
	}
	if ((state == GoombaState::WALK || destroyTimer.Elapsed() <= 5) && MEntityType::IsMarioWeapon(object.GetObjectType())) {
		return 1.0f;
	}
	return 0.0f;
}

shared_ptr<Goomba> Goomba::CreateGoomba(Vec2 fixedPos)
{
	shared_ptr<Goomba> goomba = make_shared<Goomba>();
	goomba->SetCollisionCalculator(make_shared<CollisionCalculator>(goomba));
	float height = abs((goomba->GetHitBox().bottom - goomba->GetHitBox().top));
	goomba->SetPosition(Vec2(fixedPos.x, fixedPos.y - height));
	return goomba;
}
