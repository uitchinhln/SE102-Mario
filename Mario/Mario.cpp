#include "Mario.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Events.h"

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

void Mario::OnKeyUp(int key)
{
	if (key == DIK_LEFT) {
		accelerate.x = 0;
	}
	if (key == DIK_RIGHT) {
		accelerate.x = 0;
	}
}

void Mario::OnKeyDown(int key)
{
	if (key == DIK_LEFT) {
		accelerate.x = -0.7/1000.0f;
		facing = -1;
	}
	if (key == DIK_RIGHT) {
		accelerate.x = 0.7/1000.0f;
		facing = 1;
	}	
	if (key == DIK_A) {
		accelerate.x += 0.7 / 1000.0f;
	}
	if (key == DIK_R) {
		this->Position = Vec2(1000, 800);
		this->Velocity = Vec2(0, 0);
	}
}

Mario::Mario()
{
	this->Position = Vec2(100, 300);
	this->Velocity = Vec2(0, 0);
	this->accelerate = Vec2(0, 0);
	HookEvent();
}

void Mario::InitResource()
{
}

void Mario::PhysicUpdate()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	accelerate.y = 10.0f/1000;
	Distance = Velocity * dt + 0.5 * accelerate * dt * dt;

	//update for next tick
	Velocity += accelerate * dt;
}

void Mario::Update(vector<shared_ptr<IColliable>>* coObj)
{
	vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

	// No collision occured, proceed normally
	if (coResult.size() == 0)
	{
		Position += GetDistance();
	}
	else {
		Distance = collisionCal->GetNewDistance();
		Vec2 jet = collisionCal->GetJet();
		Position += GetDistance();
		
		if (jet.x != 0) Velocity.x = 0;
		if (jet.y != 0) Velocity.y = 0;
	}

	//fixed position
	if (Position.x < 0.3) {
		Position.x = 0.3;
		Velocity.x = 0;
	}
	if (Position.y < 0.3) {
		Position.y = 0.3;
		Velocity.y = 0;
	}
}

void Mario::Render()
{
	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	string testid = "ani-big-mario-walk";
	AnimationManager::GetInstance()->Get(testid)->GetTransform()->Position = this->Position - cam;
	AnimationManager::GetInstance()->Get(testid)->Render();
}

Vec2 Mario::GetDistance()
{
	return Distance;
}

RectF Mario::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + 15 * 3, Position.y + 27 * 3);
}

bool Mario::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}
