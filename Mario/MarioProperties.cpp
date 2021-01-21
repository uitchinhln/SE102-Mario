#include "Mario.h"

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

int& Mario::GetKickCountDown()
{
	return this->kickCountDown;
}

void Mario::SetKickCountDown(int duration)
{
	this->kickCountDown = duration;
}

float& Mario::GetJumpBeginPosition()
{
	return this->jumpBeginPos;
}

void Mario::SetJumpBeginPosition(float value)
{
	this->jumpBeginPos = value;
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

bool Mario::IsLimitByCamera()
{
	return cameraLimit;
}

void Mario::SetLimitByCamera(bool value)
{
	cameraLimit = value;
}

long Mario::Invulnerable()
{
	return this->invulnerable;
}

void Mario::SetInvulnerable(long value)
{
	this->invulnerable = value;
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
	return power->GetHitBox();
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

WarpStates& Mario::GetWarpState()
{
	return this->warpState;
}

void Mario::SetWarpState(WarpStates state)
{
	this->warpState = state;
}

ObjectType Mario::GetObjectType()
{
	return power->GetMarioType();
}