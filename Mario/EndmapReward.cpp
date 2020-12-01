#include "EndmapReward.h"
#include "MEntityType.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Mario.h"

EndmapReward::EndmapReward()
{
	this->Gravity = 0;
	this->state = EndmapRewardStates::MUSHROOM;
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

void EndmapReward::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	vector<shared_ptr<IColliable>> mro;
	mro.push_back(SceneManager::GetInstance()->GetPlayer<Mario>());

	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	collisionCal->CalcPotentialCollisions(&mro, false);
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

		Velocity.y = -FLY_UP_SPEED;
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
	if (!stateTimer.IsRunning()) return;
	if (stateTimer.Elapsed() > 330) {
		this->state = EndmapRewardStates::STAR;
		stateTimer.Restart();
	}
	else if (stateTimer.Elapsed() > 220) {
		this->state = EndmapRewardStates::FLOWER;
	}
	else if (stateTimer.Elapsed() > 110) {
		this->state = EndmapRewardStates::MUSHROOM;
	}
}

void EndmapReward::Render()
{
	InitResource();

	Animation ani = this->animations["MUSHROOM"];
	switch (state)
	{
	case EndmapRewardStates::MUSHROOM:
		ani = this->animations["MUSHROOM"];
		break;
	case EndmapRewardStates::FLOWER:
		ani = this->animations["FLOWER"];
		break;
	case EndmapRewardStates::STAR:
		ani = this->animations["STAR"];
		break;
	default:
		break;
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = GetPosition() - cam;
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

bool EndmapReward::IsGetThrough(IColliable& object, Direction direction)
{
	return true;
}

float EndmapReward::GetDamageFor(IColliable& object, Direction direction)
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