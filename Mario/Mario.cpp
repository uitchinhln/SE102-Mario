#include "Mario.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Events.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"

void Mario::OnKeyUp(int key)
{
	powerUp->OnKeyUp(key);
}

void Mario::OnKeyDown(int key)
{
	powerUp->OnKeyDown(key);
	
	// Change mario power
	Vec2 fixPos = Vec2(GetHitBox().left, GetHitBox().bottom);

	switch (key)
	{
	case DIK_1:
		SetPowerUp(make_shared<Small>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_2:
		SetPowerUp(make_shared<BigMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_3:
		SetPowerUp(make_shared<FireMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_4:
		SetPowerUp(make_shared<RaccoonMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_P:
		DebugOut(L"Position: x = %f\ty = %f\n", Position.x, Position.y);
		break;
	case DIK_W:
		Position.x = 7955.459961;
		Position.y = 940.997986;
		break;
	}

}

Mario::Mario() : GameObject()
{
	this->Distance = Vec2(0, 0);
	this->Position = Vec2(100, 1200);
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

void Mario::StatusUpdate()
{
	powerUp->StatusUpdate();
}

void Mario::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	powerUp->CollisionUpdate(coObj);
}

void Mario::Update()
{
	powerUp->Update();
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
	return powerUp->GetHitBox();
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
	bool fireballAllow = object.GetObjectType() == MEntityType::MarioFireBall;
	bool tailAllow = object.GetObjectType() == MEntityType::MarioTailed;

	return fireballAllow || tailAllow;
}

float Mario::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
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