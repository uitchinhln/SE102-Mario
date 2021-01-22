#pragma once
#include "IEffect.h"
#include <functional>

class ICallbackEffect :
    public IEffect
{
protected:
    function<void(long)> callback;

    long playTime = 0;

    bool called = false;

public:
    ICallbackEffect(Vec2 pos, long playTime, function<void(long)> callback);

    virtual void Update() override;
};

