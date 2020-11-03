#pragma once
#include "AttackablePower.h"
#include "MarioFireBall.h"

class FireMario :
    public AttackablePower
{
public:
	FireMario(shared_ptr<Mario> mario);

	virtual bool CanAttack() override;

	virtual void OnAttackStart() override;

	virtual void OnAttackFinish() override;

	virtual void InAttackProgress() override;

	virtual void OnKeyDown(int key) override;

	virtual ObjectType GetMarioType() override;

protected:
	vector<shared_ptr<MarioFireBall>> bulletPool;
};

