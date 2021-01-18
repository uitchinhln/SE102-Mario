#pragma once
#include "MarioFX.h"
class GrowUpFX :
    public MarioFX
{
protected:

public:
    GrowUpFX(Vec2 pos, shared_ptr<Mario> mario);

    virtual void InitResource(Particle* particle) override;
};

