#pragma once
#include "AttackablePower.h"

class RaccoonMario :
	public AttackablePower
{
public:
	RaccoonMario(shared_ptr<Mario> mario);

	virtual void OnAttackFinish() override;

	virtual void InAttackProgress() override;

	virtual void CollisionUpdate(vector<shared_ptr<IColliable>>* coObj) override;

	virtual void MiniJumpDetect(bool forceX) override;

	virtual void PowerMeterUpdate() override;

	virtual void JumpAnimation() override;

	virtual void OnKeyDown(int key) override;

	virtual ObjectType GetMarioType() override;

protected:
	Stopwatch flyTimer;
};

