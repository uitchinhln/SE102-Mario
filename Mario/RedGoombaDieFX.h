#pragma once
#include "IEffect.h"
class RedGoombaDieFX :
    public IEffect
{
public:
    RedGoombaDieFX(Vec2 position);

    virtual void InitResource(Particle* particle);
};

