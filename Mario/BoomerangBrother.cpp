#include "BoomerangBrother.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "MarioGame.h"
#include "Mario.h"
#include "SceneManager.h"
#include "GameEvent.h"
#include "BoomerangBrotherDeathFX.h"
#include "EffectServer.h"

BoomerangBrother::BoomerangBrother()
{
	Position = VECTOR_0;

	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = BB_GRAVITY;
	this->Velocity = Vec2(BB_SPEED * facing, 0);

	this->Distance = Velocity * (float)dt;

	this->timer.Stop();
	this->moveTimer.Stop();

	for (int i = 0; i < 2; i++)
	{
		shared_ptr<Boomerang> b = make_shared<Boomerang>();
		b->SetActive(false);
		boomerangs.push_back(b);
	}
}

void BoomerangBrother::InitResource()
{
	if (animations.size() < 1) {
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-boomerang-brother-move")->Clone();
		this->animations["Attack"] = AnimationManager::GetInstance()->Get("ani-boomerang-brother-attack")->Clone();
	}
}

void BoomerangBrother::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void BoomerangBrother::PositionUpdate()
{
	UpdatedDistance = collisionCal->GetClampDistance();
	Position += UpdatedDistance;
}

void BoomerangBrother::StatusUpdate()
{
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() > 0) {
		Vec2 jet = collisionCal->GetJet();

		if (jet.x != 0) Velocity.x = -Velocity.x;
		if (jet.y != 0) Velocity.y = 0;

		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			ObjectType type = coll->Object->GetObjectType();
			if (MEntityType::IsMario(type)) {
				if (coll->SAABBResult.Direction == Direction::Bottom) {

					Position.y += size.y;
					size.y = 27;
					Position.y -= size.y;

					EffectServer::GetInstance()->SpawnEffect(make_shared<BoomerangBrotherDeathFX>(Position, Vec2(jet.x * 0.1f, 0)));
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());

					__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO, Position, GetObjectType());
				}
			}

			if (MEntityType::IsMarioWeapon(type)) {
				float damage = coll->Object->GetDamageFor(*this, coll->SAABBResult.Direction);
				if (damage > 0) {
					EffectServer::GetInstance()->SpawnEffect(make_shared<BoomerangBrotherDeathFX>(Position, Vec2(jet.x * 0.1f, -0.65f)));
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());

					if (type == MEntityType::VoidBlock) {
						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::SPACE, Position, GetObjectType());
					}
					else {
						__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO_WEAPON, Position, GetObjectType());
					}
				}
			}

			if (type == MEntityType::Boomerang) {
				bool catchMiss = rand() % 4 == 0;
				if (!catchMiss) {
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(coll->Object);
				}
			}
		}
	}
}

void BoomerangBrother::Update()
{
	DWORD dt = MarioGame::Time().ElapsedGameTime;

	if (Position.x < MarioGame::GetInstance()->GetMario()->GetPosition().x) {
		SetFacing(-1);
	}
	else {
		SetFacing(1);
	}

	if (abs(Position.x - rootPos.x) > range) {
		Velocity.x = 0;
		if (!moveTimer.IsRunning()) {
			moveTimer.Restart();
		}
		else {
			if (moveTimer.Elapsed() > 250) {
				if (Position.x - rootPos.x < 0) {
					Velocity.x = BB_SPEED;
				}
				else {
					Velocity.x = -BB_SPEED;
				}
				moveTimer.Stop();
			}
		}
	}

	if (step == 0 || step == 2) {
		if (abs(Position.x - rootPos.x) < 3) {
			if (rand() % 3 == 0) {
				Velocity.y -= 0.5f;
			}

			for each (shared_ptr<Boomerang> var in boomerangs)
			{
				if (!var->IsActive()) {
					state = BoomerangBrotherState::ATTACKING;
					break;
				}
			}

			timer.Restart();
			step++;
		}
	}
	else if (step == 1 || step == 3) {
		if (timer.Elapsed() > 300) {
			
			if (state == BoomerangBrotherState::ATTACKING && GetInCamera) {
				for each (shared_ptr<Boomerang> var in boomerangs)
				{
					if (!var->IsActive()) {
						var->Reset(GetHitBox(), facing);
						SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(var);
						state = BoomerangBrotherState::MOVING;
						break;
					}
				}
			}

			if (step == 3) {
				if (timer.Elapsed() > 3000) {
					timer.Stop();
					timer.Reset();
					step = 0;
				}
			}
			else {
				timer.Stop();
				timer.Reset();
				step = 2;
			}
		}
	}

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;
}

void BoomerangBrother::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	collisionCal->Clear();
}

void BoomerangBrother::Render(D3DCOLOR overlay)
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	Animation animation = this->animations["Walk"];

	switch (state)
	{
	case BoomerangBrotherState::MOVING:
		animation = this->animations["Walk"];
		break;
	case BoomerangBrotherState::ATTACKING:
		animation = this->animations["Attack"];
		break;
	}

	animation->GetTransform()->Position = GetPosition() - cam + size / 2;
	animation->GetTransform()->Scale.x = GetFacing();
	animation->Render(overlay);
}

ObjectType BoomerangBrother::GetObjectType()
{
    return MEntityType::BoomerangBrother;
}

RectF BoomerangBrother::GetHitBox()
{
    return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool BoomerangBrother::IsGetThrough(GameObject& object, Direction direction)
{
	if (direction == Direction::None) return true;
	if (MEntityType::IsPowerUpItem(object.GetObjectType())) return true;
	bool notBoomerangBrother = object.GetObjectType() != MEntityType::BoomerangBrother;
	return ((notBoomerangBrother) && MEntityType::IsEnemy(object.GetObjectType()));
}

float BoomerangBrother::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
		return 1.0f;
	}
    return 0.0f;
}

shared_ptr<BoomerangBrother> BoomerangBrother::CreateBoomerangBrother(Vec2 fixedPos, MapProperties& props)
{
	shared_ptr<BoomerangBrother> bb = make_shared<BoomerangBrother>();

	bb->SetCollisionCalculator(make_shared<CollisionCalculator>(bb));

	bb->rootPos = fixedPos - Vec2(0, bb->size.y);
	bb->Position = fixedPos - Vec2(0, bb->size.y);

	bb->range = props.GetFloat("Range", 48.0f);

	return bb;
}

BoomerangBrother::~BoomerangBrother()
{
}
