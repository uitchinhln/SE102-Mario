#include "Piranha.h"
#include "AnimationManager.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "Game.h"
#include "Mario.h"
#include "SmokeSpotFX.h"
#include "EffectServer.h"
#include "ScoreFX.h"
#include "GameEvent.h"
#include "MarioGame.h"

Piranha::Piranha()
{
	this->renderOrder = 125;
	this->Gravity = 0;
	this->Velocity.x = 0;

	this->movementTimer.Start();
}

void Piranha::Reset()
{
	state = PiranhaState::Reveal;

	movementState = 0;
	movementTimer.Restart();

	Position = rootPos;
}

void Piranha::InitResource()
{
	if (animations.size() < 1) {
		this->animations["Reveal"] = AnimationManager::GetInstance()->Get("ani-green-piranha-plant-attack")->Clone();
		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-green-piranha-plant-attack")->Clone();
	}
}

void Piranha::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	//if (!canCollision) return;
	if (!collidable) return;
	vector<shared_ptr<GameObject>> objs;
	for (shared_ptr<GameObject> o : *coObj) {
		if (MEntityType::IsMario(o->GetObjectType()) || MEntityType::IsMarioWeapon(o->GetObjectType())) {
			objs.push_back(o);
		}
	}
	GetCollisionCalc()->CalcPotentialCollisions(&objs, false);
	objs.clear();
}

void Piranha::RestoreCollision()
{
}

void Piranha::PositionUpdate()
{
}

void Piranha::PositionLateUpdate()
{
}

void Piranha::StatusUpdate()
{
	//if (!canCollision) return;
	if (!collidable) return;
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() != 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
				float damage = coll->Object->GetDamageFor(*this, coll->SAABBResult.Direction);
				if (damage > 0) {
					EffectServer::GetInstance()->SpawnEffect(make_shared<SmokeSpotFX>(Position));
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());

					__raise (*GameEvent::GetInstance()).EnemyDamagedEvent(__FILE__, DamgeSource::MARIO_WEAPON, GetPosition(), GetObjectType());
					break;
				}
			}
		}
	}
}

void Piranha::Update()
{
	long dt = CGame::Time().ElapsedGameTime;

	if (state == PiranhaState::Reveal)
	{
		switch (movementState)
		{
		case 0:
		{
			Position.y -= VENUS_SPEED * dt;
			if (rootPos.y - Position.y >= size.y)
			{
				Position.y = rootPos.y - size.y;
				movementState = 1;
				movementTimer.Restart();
				OnRevealed();
			}
			//canCollision = true;
			collidable = true;
		}
		break;
		case 1:
		{
			if (movementTimer.Elapsed() > VENUS_WAIT_TIME)
			{
				movementTimer.Restart();
				movementState = 2;
			}
		}
		break;
		case 2:
		{
			Position.y += VENUS_SPEED * dt;
			if (Position.y > rootPos.y)
			{
				Position.y = rootPos.y;
				movementState = 3;
				movementTimer.Restart();
				TrackPlayerPosition();
				OnHidden();
				//canCollision = false;
				collidable = false;
			}
		}
		break;
		case 3:
		{
			if (movementTimer.Elapsed() > VENUS_WAIT_TIME)
			{
				movementTimer.Restart();
				movementState = 0;
			}
		}
		break;
		}
	}
	else {
		TrackPlayerPosition();
	}
}

void Piranha::FinalUpdate()
{
	collisionCal->Clear();
}

void Piranha::Render()
{
	InitResource();
	Animation animation;

	if (state == PiranhaState::Reveal) {
		animation = animations["Reveal"];
	}
	else {
		animation = animations["Idle"];
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	animation->GetTransform()->Position = Position - cam + size / 2;
	animation->Render();
}

ObjectType Piranha::GetObjectType()
{
	return MEntityType::Piranha;
}

RectF Piranha::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Piranha::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Piranha::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType())) {
		return 1.0f;
	}
	if (MEntityType::IsMarioWeapon(object.GetObjectType())) {
		return 1.0f;
	}
	return 0.0f;
}

void Piranha::OnRevealed()
{

}

void Piranha::OnHidden()
{
}

void Piranha::OnGetInCamera()
{
	Reset();
}

void Piranha::TrackPlayerPosition()
{
	shared_ptr<Mario> player = MarioGame::GetInstance()->GetMario();
	if (!player) return;

	Vec2 marioPos = player->GetPosition();

	auto distance = abs(marioPos.x - Position.x);
	if (distance < VENUS_HIDE_DISTANCE && state == PiranhaState::Reveal)
	{
		state = PiranhaState::Hidden;
	}
	else if (distance >= VENUS_HIDE_DISTANCE && state == PiranhaState::Hidden)
	{
		state = PiranhaState::Reveal;
		movementState = 0;
		movementTimer.Restart();
	}
}

shared_ptr<Piranha> Piranha::CreatePiranha(Vec2 fixedPos)
{
	shared_ptr<Piranha> piranha = make_shared<Piranha>();
	piranha->SetCollisionCalculator(make_shared<CollisionCalculator>(piranha));
	piranha->Position = piranha->rootPos = fixedPos;
	return piranha;
}
