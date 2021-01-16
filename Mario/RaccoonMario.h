#pragma once
#include "AttackablePower.h"

class MarioTailed;

class RaccoonMario :
	public AttackablePower
{
public:
	RaccoonMario(shared_ptr<Mario> mario);

	virtual void InitResource(bool force = false) override;

	virtual void OnAttackStart() override;

	virtual void OnAttackFinish() override;

	virtual void InAttackProgress() override;

	virtual bool MiniJumpDetect(bool forceX) override;

	virtual void PowerMeterProcess() override;

	virtual void OnKeyDown(int key) override;

	virtual ObjectType GetMarioType() override;

protected:
	Stopwatch flyTimer;

	long floatEndTime = 0;

	shared_ptr<MarioTailed> tail;

	float MARIO_FLYING_UP_FORCE = 0.252f;
	float MARIO_FLOATING_SPEED = 0.021f;
};

