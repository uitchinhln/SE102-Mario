#pragma once
#include "ICallbackEffect.h"
#include "AnimationManager.h"

class CurtainFX :
    public ICallbackEffect
{
protected:
    unordered_map<string, Animation> animations;

    Vec2 size = Vec2(768, 582);

public:
    CurtainFX(long playTime, function<void(long)> callback);

    void Update() override;

    void Render(D3DCOLOR overlay);

    void InitResource();
};

