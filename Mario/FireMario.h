#pragma once
#include "AttackablePower.h"

class FireMario :
    public AttackablePower
{
public:
	FireMario(shared_ptr<Mario> mario);

	virtual void OnAttackFinish() override;

	virtual void InAttackProgress() override;

	virtual void OnKeyDown(int key) override;

	virtual ObjectType GetMarioType() override;
};

