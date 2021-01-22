#pragma once
#include "ICallbackEffect.h"
class ThreeFX :
    public ICallbackEffect
{
protected:
    unordered_map<string, Animation> animations;

    Vec2 titleS = Vec2(543, 339);
    Vec2 titleP = Vec2(122, 10);
    Vec2 _titleP = Vec2(122, -339);

    Vec2 verS = Vec2(126, 123);
    Vec2 verP = Vec2(341, 277);
    Vec2 _verP = Vec2(341, -70);

    Vec2 curS = Vec2(861, 42);
    Vec2 curP = Vec2(0, 0);

    int step = 0;

    long timer = 0;

public:
    ThreeFX(Vec2 Pos, long playTime, function<void(long)> callback);

    void Update() override;

    void Render(D3DCOLOR overlay);

    void InitResource();
};
