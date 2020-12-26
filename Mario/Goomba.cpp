#include "Goomba.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"

Goomba::Goomba()
{
	destroyTimer.Stop();
	state = GoombaState::WALK;

	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = GB_GRAVITY;
	this->Velocity = Vec2(GB_SPEED * facing, 0);

	this->Distance = Velocity * (float)dt;
}

void Goomba::InitResource()
{
	if (animations.size() < 1) {
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-goomba-walk")->Clone();
		this->animations["Die"] = AnimationManager::GetInstance()->Get("ani-goomba-die")->Clone();
		this->animations["Explode"] = AnimationManager::GetInstance()->Get("ani-goomba-idle")->Clone();

		this->animations["Explode"]->GetTransform()->Scale.y = -1;
	}
}

void Goomba::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (state != GoombaState::WALK) return;

	GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void Goomba::PositionUpdate()
{
	if (state == GoombaState::WALK) {
		UpdatedDistance = collisionCal->GetClampDistance();
	}
	else {
		UpdatedDistance = Distance;
	}
	Position += UpdatedDistance;
}

void Goomba::StatusUpdate()
{
	if (state != GoombaState::WALK) return;

	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() > 0) {
		Vec2 jet = collisionCal->GetJet();

		if (jet.x != 0) Velocity.x = -Velocity.x;
		if (jet.y != 0) Velocity.y = 0;

		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMario(coll->Object->GetObjectType())) {
				if (coll->SAABBResult.Direction == Direction::Bottom) {
					state = GoombaState::DIE;
					Velocity = VECTOR_0;
					Gravity = 0;

					Position.y += size.y;
					size.y = 27;
					Position.y -= size.y;

					if (!destroyTimer.IsRunning()) {
						destroyTimer.Restart();
					}
				}
			}

			if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
				float damage = coll->Object->GetDamageFor(*this, coll->SAABBResult.Direction);
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
	if (GetPosition().x < 0.3 - size.x || GetPosition().y < 0.3 - size.y || GetPosition().x > mapBound.x || GetPosition().y > mapBound.y) {
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

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;
}

void Goomba::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	facing = Velocity.x > 0 ? 1 : -1;
	collisionCal->Clear();
}

void Goomba::Render()
{
	InitResource();

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
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
	return hitbox;
}

bool Goomba::IsGetThrough(GameObject& object, Direction direction)
{
	if (direction == Direction::None) return true;
	if (MEntityType::IsPowerUpItem(object.GetObjectType())) return true;
	//bool koopasImp = object.GetObjectType() == MEntityType::KoopasImposter;
	bool notGoomba = object.GetObjectType() != MEntityType::Goomba;
	return state != GoombaState::WALK || ((notGoomba) && MEntityType::IsEnemy(object.GetObjectType()));
}

float Goomba::GetDamageFor(GameObject& object, Direction direction)
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

Goomba::~Goomba()
{
	//DebugOut(L"Huy goomba\n");
}
