#include "Mario.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Events.h"
#include "Small.h"

void Mario::OnKeyUp(int key)
{
	powerUp->OnKeyUp(key);
}

void Mario::OnKeyDown(int key)
{
	powerUp->OnKeyDown(key);
}

Mario::Mario() : CGameObject()
{
	this->Distance = Vec2(0, 0);
	this->Position = Vec2(100, 300);
	this->Velocity = Vec2(0, 0);
	this->accelerate = Vec2(0, 0);
	HookEvent();
}

void Mario::InitResource()
{
	this->SetCollisionCalculator(make_shared<CollisionCalculator>(shared_from_this()));
	this->powerUp = make_shared<Small>(shared_from_this());
	this->Gravity = 0.00093f;
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
	return RectF(Position.x, Position.y, Position.x + 15 * 3, Position.y + 27 * 3);
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

int Mario::GetObjectType()
{
	return 0;
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