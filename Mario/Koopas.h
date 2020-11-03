#pragma once
#include "GameObject.h"
#include "DefaultKoopas.h"

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

	virtual void SetPower(shared_ptr<DefaultKoopas> power);

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(IColliable& object, Direction direction) override;

	virtual float GetDamageFor(IColliable& object, Direction direction) override;

	static shared_ptr<Koopas> CreateKoopas(Vec2 fixedPos = Vec2(100, 100));

protected:
	shared_ptr<DefaultKoopas> power;

	KoopasLiveStates state;

	Stopwatch destroyTimer;

	float KP_DESTROY_DELAY = 500;
};

