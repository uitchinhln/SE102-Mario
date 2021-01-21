#pragma once
#include "IEffect.h"
#include <functional>

class ICallbackEffect :
    public IEffect
{
protected:
    function<void(long)> callback;

    long playTime = 0;

public:
    ICallbackEffect(Vec2 pos, long playTime, function<void(long)> callback);

    virtual void Update() override;
};

