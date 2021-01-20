#pragma once
#include "IEffect.h"
#include "GameEnum.h"

class CSprite;

class ScoreFX :
    public IEffect
{
public:
    ScoreFX(Vec2 pos, Score score);

    virtual CSprite* GetSprite(Score score);

    virtual void InitResource(Particle* particle) override;

    ~ScoreFX();
};

