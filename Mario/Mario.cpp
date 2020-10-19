#include "Mario.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"

Mario::Mario()
{
	this->Position = Vec2(100, 800);
	this->Velocity = Vec2(0, 0);
}

void Mario::InitResource()
{
}

void Mario::Update(vector<shared_ptr<IColliable>>* coObj)
{
	Velocity.y = 0.005 * CGame::Time().ElapsedGameTime.Milliseconds();

	vector<CollisionResult> coResult;
	coResult.clear();

	CalcPotentialCollisions(coObj, coResult);

	// No collision occured, proceed normally
	if (coResult.size() == 0)
	{
		Position += GetDistance();
	}
	else {
		Velocity.y = 0;
	}
}

void Mario::Render()
{
	string testid = "ani-big-mario-walk";
	AnimationManager::GetInstance()->Get(testid)->GetTransform()->Position = this->Position - Vec2(0, 735);
	AnimationManager::GetInstance()->Get(testid)->Render();
}

Vec2 Mario::GetDistance()
{
	return Velocity*CGame::Time().ElapsedGameTime.Milliseconds();
}

RectF Mario::GetBoundingBox()
{
	return RectF(Position.x, Position.y, Position.x + 48, Position.y + 96);
}

bool Mario::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}
