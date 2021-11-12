#pragma once
#include "IEffect.h"
#include "Text.h"
#include "GameEnum.h"

class PlaySceneFinishFX :
    public IEffect
{
protected:
    Text line1;
    Text line2;

    Sprite card;

    Transform trans;

    long line2Delay = 700;

public:
    PlaySceneFinishFX(Vec2 position, long playTime, CardType card);

    virtual void Update() override;

    virtual void Render(D3DXCOLOR overlay) override;
};

