#include "Mario.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Events.h"
#include "Small.h"
#include "StateManager.h"

void Mario::OnKeyUp(int key)
{
	powerUp->OnKeyUp(key);
}

void Mario::OnKeyDown(int key)
{
	powerUp->OnKeyDown(key);
	switch (key)
	{
	case DIK_1:
		SetPowerUp(StateManager::GetInstance()->Get("SmallMario"));
		break;
	case DIK_2:
		SetPowerUp(StateManager::GetInstance()->Get("BigMario"));
		this->Position.y -= 50;
		break;
	case DIK_3:
		SetPowerUp(StateManager::GetInstance()->Get("FireMario"));
		this->Position.y -= 50;
		break;
	case DIK_4:
		SetPowerUp(StateManager::GetInstance()->Get("RaccoonMario"));
		this->Position.y -= 50;
		break;
	default:
		break;
	}
}

Mario::Mario() : CGameObject()
{
	this->Distance = Vec2(0, 0);
	this->Position = Vec2(100, 300);
	this->Velocity = Vec2(0, 0);
	this->accelerate = Vec2(0, 0);
	this->Gravity = 0.00093f;
	HookEvent();
}

void Mario::SetPowerUp(shared_ptr<MarioPowerUp> powerUp)
{
	this->powerUp = powerUp;
}

void Mario::InitResource()
{
	//this->SetCollisionCalculator(make_shared<CollisionCalculator>(shared_from_this()));
}

void Mario::Update(vector<shared_ptr<IColliable>>* coObj)
{
	powerUp->Update(coObj);
}

void Mario::Render()
{
	powerUp->Render();
}

float& Mario::GetDrag()
{
	return this->drag;
}

void Mario::SetDrag(float drag)
{
	this->drag = drag;
}

int& Mario::GetSkid()
{
	return this->skid;
}

void Mario::SetSkid(int skid)
{
	this->skid = skid;
}

int& Mario::CanHighJump()
{
	return this->canHighJump;
}

void Mario::SetCanHighJump(bool value)
{
	this->canHighJump = value;
}

float& Mario::GetPowerMeter()
{
	return this->powerMeter;
}

void Mario::SetPowerMeter(float value)
{
	this->powerMeter = value;
}

bool Mario::IsOnGround()
{
	return onGround;
}

void Mario::SetOnGround(bool value)
{
	onGround = value;
}

Vec2& Mario::GetDistance()
{
	return Distance;
}

Vec2& Mario::GetAccelerate()
{
	return accelerate;
}

RectF Mario::GetHitBox()
{
	return powerUp->GetBoundingBox();
}

MovingStates& Mario::GetMovingState()
{
	return this->movingState;
}

void Mario::SetMovingState(MovingStates state)
{
	this->movingState = state;
}

JumpingStates& Mario::GetJumpingState()
{
	return this->jumpingState;
}

void Mario::SetJumpingState(JumpingStates state)
{
	this->jumpingState = state;
}

ObjectType Mario::GetObjectType()
{
	return powerUp->GetMarioType();
}

bool Mario::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

void Mario::HookEvent()
{
	__hook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	__hook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}

void Mario::UnHookEvent()
{
	__unhook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	__unhook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}