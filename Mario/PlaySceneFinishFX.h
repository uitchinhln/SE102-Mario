#pragma once
#include "IEffect.h"
#include "Text.h"

enum class CardType;

class PlaySceneFinishFX :
    public IEffect
{
protected:
    Text line1;
    Text line2;

    Sprite card;

    Transform trans;

    long line2Delay = 600;

public:
    PlaySceneFinishFX(Vec2 position, CardType card);

    virtual void Update() override;

    virtual void Render() override;
};

