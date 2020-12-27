#include "Coin.h"
#include "MEntityType.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"

Coin::Coin()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = 0;
	this->Velocity = Vec2(0, Gravity);

	this->Distance = Velocity * (float)dt;
}

void Coin::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["CoinDefault"] = AnimationManager::GetInstance()->Get("ani-coin");
		this->animations["CoinFreeze"] = AnimationManager::GetInstance()->Get("ani-idle-coin");
		this->animations["BricksDefault"] = AnimationManager::GetInstance()->Get("ani-brick");
		this->animations["BricksFreeze"] = AnimationManager::GetInstance()->Get("ani-brick-time-freeze");
	}
}

void Coin::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void Coin::PositionUpdate()
{
}

void Coin::StatusUpdate()
{
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() > 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (state == CoinState::COIN && MEntityType::IsMario(coll->Object->GetObjectType())) {
				SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
			}
		}
	}
}

void Coin::Update()
{
}

void Coin::FinalUpdate()
{
}

void Coin::Render()
{
	InitResource();

	Animation ani = animations["CoinDefault"];

	switch (state)
	{
	case CoinState::COIN:
		ani = this->isFreeze ? animations["CoinFreeze"] : animations["CoinDefault"];
		break;
	case CoinState::BRICK:
		ani = this->isFreeze ? animations["BricksFreeze"] : animations["BricksDefault"];
		break;
	}

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	ani->GetTransform()->Position = Position - cam;
	ani->Render();
}

ObjectType Coin::GetObjectType()
{
	return MEntityType::Coin;
}

RectF Coin::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Coin::IsGetThrough(GameObject& object, Direction direction)
{
	return state == CoinState::COIN;
}

float Coin::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

shared_ptr<Coin> Coin::CreateCoin(Vec2 pos, CoinState state)
{
	shared_ptr<Coin> coin = make_shared<Coin>();
	coin->SetCollisionCalculator(make_shared<CollisionCalculator>(coin));
	coin->SetPosition(pos);
	coin->state = state;
	return coin;
}
