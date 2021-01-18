#pragma once
#include "Utils.h"
#include "Particle.h"

class IEffect
{
protected:
	Vec2 Position;

	long playtimeLeft = 0;

	vector<Particle*> particles;

public:
	bool Active = true;

public:
	IEffect(Vec2 Position);

	IEffect(Vec2 Position, long playtime);

	virtual void InitResource(Particle* particle);

	virtual Vec2 GetPosition();

	virtual long GetPlayTimeLeft();

	virtual void SetPlayTimeLeft(long value);

	virtual void Update();

	virtual void Render();

	~IEffect();
};

