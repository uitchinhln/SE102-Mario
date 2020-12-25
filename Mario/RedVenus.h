#pragma once
#include "Venus.h"
class RedVenus :
    public Venus
{
public:
    RedVenus();

    virtual void InitResource() override;

    static shared_ptr<RedVenus> CreateRedVenus(Vec2 fixedPos);
};

