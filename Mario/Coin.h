#pragma once
#include "GameObject.h"
#include "MapProperties.h"
#include "StopWatch.h"
#include "GameEnum.h"

class Coin :
    public GameObject,
	public enable_shared_from_this<Coin>
{
public:
    Coin();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual CoinState& State();

	virtual void Reverser();

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Coin> CreateCoin(Vec2 pos, CoinState state);

protected:
	CoinState state = CoinState::COIN;

	bool isFreeze = false;

	Stopwatch reverseTimer;

	int reverseDuration = 10000;

	Vec2 size = Vec2(48, 48);
};

