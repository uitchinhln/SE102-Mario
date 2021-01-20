#pragma once
#include "GameObject.h"
#include "DefaultKoopas.h"

class Mario;

enum class KoopasLifeStates {
	ALIVE,
	DIE
};

class Koopas :
    public GameObject,
	public enable_shared_from_this<Koopas>
{
public:
	Koopas();

	virtual KoopasLifeStates& GetLiveState();

	virtual weak_ptr<Mario> GetHolder();

	virtual void SetHolder(shared_ptr<Mario> koopas);

	virtual void ClearHolder();

	virtual void SetPower(shared_ptr<DefaultKoopas> power);

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void StatusUpdate() override;

	virtual void Update() override;

	virtual void FinalUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	virtual void OnLostCamera() override;

	static shared_ptr<Koopas> CreateKoopas(Vec2 fixedPos = Vec2(100, 100));

	~Koopas();

protected:
	weak_ptr<Mario> koopas;

	shared_ptr<DefaultKoopas> power;

	KoopasLifeStates state;

public:

	bool OnGround = false;
};

