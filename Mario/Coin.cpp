#include "Coin.h"
#include "MEntityType.h"
#include "Game.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "EffectServer.h"
#include "BrickDebrisFX.h"
#include "GameEvent.h"

Coin::Coin()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = 0;
	this->Velocity = Vec2(0, Gravity);

	this->Distance = Velocity * (float)dt;

	this->reverseTimer.Reset();
	this->reverseTimer.Stop();
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
				__raise (*GameEvent::GetInstance()).PlayerCoinChangeEvent(__FILE__, 1);
			}

			if (state == CoinState::BRICK) {
				if (MEntityType::IsMario(coll->Object->GetObjectType()) && coll->SAABBResult.Direction == Direction::Top) {

					DebugOut(L"Mario %d\n", CGame::Time().TotalGameTime);

					if (coll->Object->GetObjectType() == MEntityType::SmallMario) continue;
					EffectServer::GetInstance()->SpawnEffect(make_shared<BrickDebrisFX>(Position));
					active = false;
					//SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
				}
				if (coll->Object->GetObjectType() == MEntityType::KoopasImposter && ToVector(coll->SAABBResult.Direction).x != 0) {
					EffectServer::GetInstance()->SpawnEffect(make_shared<BrickDebrisFX>(Position));
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
				}
				if (coll->Object->GetObjectType() == MEntityType::MarioTailed) {
					EffectServer::GetInstance()->SpawnEffect(make_shared<BrickDebrisFX>(Position));
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
				}
			}
		}
	}
}

void Coin::Update()
{
	if (reverseTimer.IsRunning() && reverseTimer.Elapsed() >= reverseDuration) {
		reverseTimer.Reset();
		reverseTimer.Stop();

		if (state == CoinState::COIN) {
			state = CoinState::BRICK;
		}
		else {
			state = CoinState::COIN;
		}

		this->isFreeze = false;
	}
}

void Coin::FinalUpdate()
{
	collisionCal->Clear();
}

void Coin::Render(D3DCOLOR overlay)
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

	ani->GetTransform()->Position = Position - cam + size / 2;
	ani->Render(overlay);
}

ObjectType Coin::GetObjectType()
{
	if (state == CoinState::BRICK)
		return MEntityType::Brick;
	return MEntityType::Coin;
}

RectF Coin::GetHitBox()
{
	if (state == CoinState::BRICK) {
		return RectF(Position.x, Position.y, Position.x + 48, Position.y + 48);
	}
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

CoinState& Coin::State()
{
	return state;
}

void Coin::Reverser()
{
	if (state == CoinState::COIN) {
		state = CoinState::BRICK;
	}
	else {
		state = CoinState::COIN;
	}
	isFreeze = true;
	reverseTimer.Start();
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
