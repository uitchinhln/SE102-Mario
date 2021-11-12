#pragma once
#include "IEffect.h"
class SmokeSpotFX :
    public IEffect
{
protected:
    long runTime = 0;
public:
    SmokeSpotFX(Vec2 position);

    virtual void Update() override;

    virtual void Render(D3DXCOLOR overlay) override;

    virtual void InitResource(Particle* particle);

    ~SmokeSpotFX();
};

