#pragma once
#include "GameObject.h"
#include "MapProperties.h"
#include "StopWatch.h"
#include "Boomerang.h"

enum class BoomerangBrotherState {
	MOVING,
	ATTACKING
};

class BoomerangBrother :
    public GameObject,
	public enable_shared_from_this<BoomerangBrother>
{
public:
	BoomerangBrother();

	virtual void InitResource() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DXCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<BoomerangBrother> CreateBoomerangBrother(Vec2 fixedPos, MapProperties& props);

	~BoomerangBrother();

protected:
	Vec2 size = Vec2(49, 66);
	Vec2 rootPos = VECTOR_0;

	int step = 0;
	Stopwatch timer;
	Stopwatch moveTimer;

	BoomerangBrotherState state = BoomerangBrotherState::MOVING;

	vector<shared_ptr<Boomerang>> boomerangs;

	float range = 48.0f;

	float BB_GRAVITY = 0.002f;
	float BB_SPEED = 0.08f;
};

