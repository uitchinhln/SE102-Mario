#pragma once
#include "IEffect.h"
class GoombaDieFX :
    public IEffect
{
public:
    GoombaDieFX(Vec2 position);

    virtual void InitResource(Particle* particle);
};

