#pragma once
#include "IEffect.h"
class BrickDebrisFX :
    public IEffect
{
public:
    BrickDebrisFX(Vec2 pos);

    virtual void InitResource(Particle* particle);
};

