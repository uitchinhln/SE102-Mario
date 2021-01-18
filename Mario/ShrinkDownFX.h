#pragma once
#include "MarioFX.h"
class ShrinkDownFX :
    public MarioFX
{
protected:

public:
    ShrinkDownFX(Vec2 pos, shared_ptr<Mario> mario);

    virtual void InitResource(Particle* particle) override;
};

