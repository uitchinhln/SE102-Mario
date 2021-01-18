#pragma once
#include "MovingShell.h"
class RedMovingShell :
    public MovingShell
{
public:
    RedMovingShell(shared_ptr<Koopas> koopas, bool flip = false);

    virtual void InitResource(bool force = false) override;

    virtual void StatusUpdate() override;

    virtual void OnDeath(Vec2 veloc) override;
};

