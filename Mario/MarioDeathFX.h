#pragma once
#include "IEffect.h"
class MarioDeathFX :
    public IEffect
{
public:
    MarioDeathFX(Vec2 pos);

    virtual void Update() override;

    virtual void InitResource(Particle* particle) override;
};

