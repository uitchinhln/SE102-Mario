#pragma once
#include "IEffect.h"
class TreeFX :
    public IEffect
{
public:
    TreeFX(Vec2 position);

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;

    virtual void InitResource(Particle* particle);
};

