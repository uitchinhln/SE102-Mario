#pragma once
#include "IEffect.h"

enum class CardType {
    FLOWER,
    MUSHROOM,
    STAR
};

class CardFX :
    public IEffect
{
protected:
    CardType type;

public:
    CardFX(Vec2 pos, Vec2 velocity, CardType type);

    virtual void InitResource(Particle* particle) override;
};

