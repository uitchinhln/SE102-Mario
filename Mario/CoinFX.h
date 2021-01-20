#pragma once
#include "IEffect.h"
#include "ScoreFX.h"

class CoinFX :
    public IEffect
{
protected:
    Animation animation;
    Vec2 size = Vec2(48, 48);
    Vec2 rootPos;
    Score score;

    int stage = 0;

    float flyUpSpeed = 0.6;
    float fallDownSpeed = 0;

    float gravity = 0.002f;

public:
    CoinFX(Vec2 pos, Score score);

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;
};

