#include "Venus.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"
#include "Mario.h"
#include "Vec2Utils.h"

Venus::Venus()
{
	this->renderOrder = 125;
	this->Gravity = 0;
	this->Velocity.x = 0;
	this->Velocity.y = 0;
	this->Distance.y = 0;
	this->Distance.y = 0;

	this->shootTimer.Start();
	this->movementTimer.Start();
}

void Venus::Reset()
{
	this->shootTimer.Restart();
	this->movementTimer.Restart();
	canCollision = true;

	state = VenusState::Reveal;

	targetLocking = 1;
	movementState = 0;
	verticalDirection = 1;
}

void Venus::InitData()
{
	for (int i = 0; i < VENUS_N_POOLED_BULLETS; i++) {
		shared_ptr<VenusFireball> ball = make_shared<VenusFireball>();
		ball->SetActive(false);
		fireballs.push_back(ball);
	}
}

void Venus::InitResource()
{
	if (animations.size() < 1) {
		this->animations["RevealHeadUp"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headup")->Clone();
		this->animations["RevealHeadDown"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headdown")->Clone();
		this->animations["IdleHeadUp"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headup-idle")->Clone();
		this->animations["IdleHeadDown"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headdown-idle")->Clone();
	}
}

void Venus::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	if (!canCollision) return;
	vector<shared_ptr<GameObject>> objs;
	for (shared_ptr<GameObject> o : *coObj) {
		if (MEntityType::IsMario(o->GetObjectType()) || MEntityType::IsMarioWeapon(o->GetObjectType())) {
			objs.push_back(o);
		}
	}
	GetCollisionCalc()->CalcPotentialCollisions(&objs, false);
	objs.clear();
}

void Venus::RestoreCollision()
{
	//Do nothing
}

void Venus::PositionUpdate()
{
	//Do nothing
}

void Venus::PositionLateUpdate()
{
	//Do nothing
}

void Venus::StatusUpdate()
{
	if (!canCollision) return;
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() != 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
				float damage = coll->Object->GetDamageFor(*this, coll->SAABBResult.Direction);
				if (damage > 0) {
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
					break;
				}
			}
		}
	}
}

void Venus::MovingUpdate()
{
	long dt = CGame::Time().ElapsedGameTime;

	if (state == VenusState::Reveal)
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
			canCollision = true;
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
				canCollision = false;
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

void Venus::Update()
{
	MovingUpdate();

	shared_ptr<Mario> player = SceneManager::GetInstance()->GetPlayer<Mario>();
	Vec2 marioPos = player->GetPosition();

	if (movementState <= 1) {
		facing = marioPos.x < Position.x ? -1 : 1;
		if (!targetLocking) verticalDirection = marioPos.y < Position.y ? -1 : 1;
	}

	long dt = CGame::Time().ElapsedGameTime;

	if (targetLocking && this->shootTimer.Elapsed() > VENUS_SHOOT_WAIT_TIME) {
		shootTimer.Restart();
		targetLocking = false;

		for each (shared_ptr<VenusFireball> fireball in fireballs)
		{
			if (!fireball->IsActive()) {
				fireball->Reset(GetHitBox(), facing);
				Vec2 ballPos = fireball->GetPosition();

				RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();

				if (ballPos.x >= cam.left && ballPos.y >= cam.top && ballPos.x <= cam.right && ballPos.y <= cam.bottom) {
					SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(fireball);
					break;
				}
			}
		}
	}
}

void Venus::FinalUpdate()
{
	//Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	collisionCal->Clear();
}

void Venus::Render()
{
	InitResource();
	Animation animation;

	switch (movementState)
	{
	case 0:
	case 2:
		animation = verticalDirection < 0 ? animations["RevealHeadUp"] : animations["RevealHeadDown"];
		break;
	case 1:
	case 3:
		animation = verticalDirection < 0 ? animations["IdleHeadUp"] : animations["IdleHeadDown"];
		break;
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	animation->GetTransform()->Scale.x = -facing;
	animation->GetTransform()->Position = Position - cam;
	animation->Render();
}

ObjectType Venus::GetObjectType()
{
	return MEntityType::Venus;
}

RectF Venus::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Venus::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Venus::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType())) {
		return 1.0f;
	}
	if (MEntityType::IsMarioWeapon(object.GetObjectType())) {
		return 1.0f;
	}
	return 0.0f;
}

void Venus::OnRevealed()
{
	targetLocking = true;
	shootTimer.Restart();
}

void Venus::OnHidden()
{

}

void Venus::OnGetInCamera()
{
	for each (shared_ptr<VenusFireball> fireball in fireballs)
	{
		if (!fireball->IsActive()) {
			Reset();
			break;
		}
	}
}

void Venus::TrackPlayerPosition()
{
	shared_ptr<Mario> player = SceneManager::GetInstance()->GetPlayer<Mario>();
	if (!player) return;

	Vec2 marioPos = player->GetPosition();
	facing = marioPos.x < Position.x ? -1 : 1;

	auto distance = abs(marioPos.x - Position.x);
	if (distance < VENUS_HIDE_DISTANCE && state == VenusState::Reveal)
	{
		state = VenusState::Hidden;
	}
	else if (distance >= VENUS_HIDE_DISTANCE && state == VenusState::Hidden)
	{
		state = VenusState::Reveal;
		movementState = 0;
		movementTimer.Restart();
	}

}

shared_ptr<Venus> Venus::CreateVenus(Vec2 fixedPos)
{
	shared_ptr<Venus> venus = make_shared<Venus>();
	venus->SetCollisionCalculator(make_shared<CollisionCalculator>(venus));
	venus->Position = venus->rootPos = fixedPos;
	venus->InitData();
	return venus;
}
