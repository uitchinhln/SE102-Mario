#pragma once
#include "IEffect.h"
class HitStarFX :
    public IEffect
{
protected: 
    long runTime = 0;
public:
    HitStarFX(Vec2 position);

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;

    virtual void InitResource(Particle* particle);
};

