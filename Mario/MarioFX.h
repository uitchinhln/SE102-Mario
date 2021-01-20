#pragma once
#include "IEffect.h"
#include "StopWatch.h"

class Mario;

class MarioFX :
    public IEffect
{
protected:
    shared_ptr<Mario> mario;

    Stopwatch timer = Stopwatch(TimeMode::SYSTEM_TIME);

public:
    MarioFX(Vec2 pos, shared_ptr<Mario> mario);

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;
};

