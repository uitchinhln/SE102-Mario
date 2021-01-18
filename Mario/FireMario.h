#pragma once
#include "AttackablePower.h"
#include "MarioFireBall.h"

class FireMario :
    public AttackablePower
{
public:
	FireMario(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual bool CanAttack() override;

	virtual void OnAttackStart() override;

	virtual void OnAttackFinish() override;

	virtual void InAttackProgress() override;

	virtual ObjectType GetMarioType() override;


	virtual void OnKeyDown(int key) override;

	virtual void OnDamaged(float damage) override;

	virtual void OnPowerUp(ObjectType powerType) override;

protected:
	vector<shared_ptr<MarioFireBall>> bulletPool;
};

