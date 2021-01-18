#pragma once
#include "IEffect.h"
class RedGoombaExplodeFX :
    public IEffect
{
public:
    RedGoombaExplodeFX(Vec2 pos, Vec2 velocity);

    virtual void InitResource(Particle* particle) override;
};

