#pragma once
#include "MarioFX.h"
class TransformFX :
    public MarioFX
{
protected:

public:
    TransformFX(Vec2 pos, shared_ptr<Mario> mario);

    virtual void InitResource(Particle* particle) override;
};

