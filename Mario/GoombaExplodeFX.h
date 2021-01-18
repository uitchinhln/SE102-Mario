#pragma once
#include "IEffect.h"
class GoombaExplodeFX :
    public IEffect
{
public:
    GoombaExplodeFX(Vec2 pos, Vec2 velocity);

    virtual void InitResource(Particle* particle) override;

    ~GoombaExplodeFX();
};

