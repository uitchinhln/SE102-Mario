#pragma once
#include "IEffect.h"
class KoopasDieFX :
    public IEffect
{
private:
    string aniId = "";

public:
    KoopasDieFX(Vec2 pos, Vec2 velocity, string aniId);

    virtual void InitResource(Particle* particle) override;
};

