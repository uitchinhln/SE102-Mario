#pragma once
#include "DefaultRedGoomba.h"
#include "StopWatch.h"

class JumpingRedGoomba :
    public DefaultRedGoomba
{
public:
    JumpingRedGoomba(shared_ptr<RedGoomba> holder);

	virtual void InitResource() override;

	virtual void Update() override;

	virtual void StatusUpdate() override;

	virtual void Render(D3DCOLOR overlay) override;

protected:
	float KOOPAS_JUMP_FORCE = 0.55f;

	int jumpStep = 0;

	Stopwatch jumpTimer;

	const float RED_PARAGOOMBA_HIGH_JUMP = 0.6f;
	const float RED_PARAGOOMBA_LOW_JUMP = 0.23f;
	const int RED_PARAGOOMBA_BREAK_TIME = 720; // miliseconds = s / v = 72 / 0.1
};

