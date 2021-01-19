#include "EndmapReward.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "CardFX.h"
#include "EffectServer.h"

EndmapReward::EndmapReward()
{
	this->Gravity = 0;
	this->state = CardType::Mushroom;
	stateTimer.Start();
}

void EndmapReward::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["MUSHROOM"] = AnimationManager::GetInstance()->Get("ani-super-mushroom")->Clone();
		this->animations["FLOWER"] = AnimationManager::GetInstance()->Get("ani-fire-flower")->Clone();
		this->animations["STAR"] = AnimationManager::GetInstance()->Get("ani-star-man-white")->Clone();
	}
}

void EndmapReward::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	vector<shared_ptr<GameObject>> mro;
	mro.push_back(SceneManager::GetInstance()->GetPlayer<Mario>());

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	collisionCal->CalcPotentialCollisions(&mro, false);
}

void EndmapReward::CollisionDoubleFilter()
{
}

void EndmapReward::PositionLateUpdate()
{
}

bool EndmapReward::HasCollideWith(DWORD id)
{
	return true;
}

void EndmapReward::FinalUpdate()
{
	GameObject::FinalUpdate();
	collisionCal->Clear();
}

void EndmapReward::StatusUpdate()
{
	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();
	if (collisionCal->GetLastResults().size() > 0) {
		shared_ptr<Mario> mario = SceneManager::GetInstance()->GetPlayer<Mario>();

		this->holder = mario;
		this->holder->SetLockController(true);
		this->holder->SetFacing(1);

		this->stateTimer.Stop();
		Velocity = VECTOR_0;
		Visible = false;

		EffectServer::GetInstance()->SpawnEffect(make_shared<CardFX>(Position, Vec2(0, -FLY_UP_SPEED), state));
	}

	if (this->holder != nullptr) {
		if (holder->IsOnGround()) {
			holder->SetVelocity(Vec2(MARIO_RUN_SPEED, FLY_UP_SPEED));
			holder->SetMovingState(MovingStates::WALK);
		}
		else {
			holder->SetVelocity(Vec2(0, FLY_UP_SPEED * 2));
			holder->SetJumpingState(JumpingStates::FALLING);
		}
	}
}

void EndmapReward::Update()
{
	if (!stateTimer.IsRunning()) {
		collidable = false;
		return;
	}
	if (stateTimer.Elapsed() > 330) {
		this->state = CardType::Star;
		stateTimer.Restart();
	}
	else if (stateTimer.Elapsed() > 220) {
		this->state = CardType::Flower;
	}
	else if (stateTimer.Elapsed() > 110) {
		this->state = CardType::Mushroom;
	}
}

void EndmapReward::Render()
{
	InitResource();

	Animation ani = this->animations["MUSHROOM"];
	switch (state)
	{
	case CardType::Mushroom:
		ani = this->animations["MUSHROOM"];
		break;
	case CardType::Flower:
		ani = this->animations["FLOWER"];
		break;
	case CardType::Star:
		ani = this->animations["STAR"];
		break;
	default:
		break;
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = GetPosition() - cam + size / 2;
	ani->Render();
}

ObjectType EndmapReward::GetObjectType()
{
	return MEntityType::EndmapReward;
}

RectF EndmapReward::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool EndmapReward::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float EndmapReward::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<EndmapReward> EndmapReward::CreateEndmapReward(Vec2 fixedPos)
{
	shared_ptr<EndmapReward> reward = make_shared<EndmapReward>();
	reward->SetCollisionCalculator(make_shared<CollisionCalculator>(reward));
	reward->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	return reward;
}