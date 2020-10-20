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
		Velocity.x = 0;
	}
	if (key == DIK_RIGHT) {
		Velocity.x = 0;
	}
}

void Mario::OnKeyDown(int key)
{
	if (key == DIK_LEFT) {
		Velocity.x += -0.2;
	}
	if (key == DIK_RIGHT) {
		Velocity.x += 0.2;
	}
	if (key == DIK_R) {
		this->Position = Vec2(100, 800);
		this->Velocity = Vec2(0, 0);
	}
}

Mario::Mario()
{
	this->Position = Vec2(100, 0);
	this->Velocity = Vec2(0, 0);
	HookEvent();
}

void Mario::InitResource()
{
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
		Vec2 d = collisionCal->GetNewDistance();
		Vec2 jet = collisionCal->GetJet();
		Position += d;
		if (Position.x < 0.3) Position.x = 0.3;
		if (Position.y < 0.3) Position.y = 0.3;
		
		if (jet.x != 0) Velocity.x = 0;
		if (jet.y != 0) Velocity.y = 0;
	}
	Velocity.y += 0.002f * CGame::Time().ElapsedGameTime;
	//DebugOut(L"Pos(%f, %f)\tVeloc(%f, %f)\n", Position.x, Position.y, Velocity.x, Velocity.y);
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
	return Velocity*CGame::Time().ElapsedGameTime;
}

RectF Mario::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + 15 * 3, Position.y + 27 * 3);
}

bool Mario::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}
