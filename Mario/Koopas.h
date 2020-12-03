#pragma once
#include "GameObject.h"
#include "DefaultKoopas.h"
#include "StopWatch.h"

class Mario;

enum class KoopasLiveStates {
	ALIVE,
	DIE
};

class Koopas :
    public GameObject
{
public:
	Koopas();
	
	virtual Stopwatch& GetDestroyTimer();

	virtual KoopasLiveStates& GetLiveState();

	virtual weak_ptr<Mario> GetHolder();

	virtual void SetHolder(shared_ptr<Mario> koopas);

	virtual void ClearHolder();

	virtual void SetPower(shared_ptr<DefaultKoopas> power);

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Koopas> CreateKoopas(Vec2 fixedPos = Vec2(100, 100));

	~Koopas();

protected:
	weak_ptr<Mario> koopas;

	shared_ptr<DefaultKoopas> power;

	KoopasLiveStates state;

	Stopwatch destroyTimer;

	float KP_DESTROY_DELAY = 500;
};

