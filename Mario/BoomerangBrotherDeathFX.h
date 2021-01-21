#pragma once
#include "IEffect.h"
class BoomerangBrotherDeathFX :
    public IEffect
{
public:
    BoomerangBrotherDeathFX(Vec2 pos, Vec2 velocity);

    virtual void InitResource(Particle* particle);
};

