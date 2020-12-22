#pragma once
#include "CrouchKoopas.h"

class RedCrouchKoopas :
    public CrouchKoopas
{
public:
    RedCrouchKoopas(shared_ptr<Koopas> koopas, bool flip = false);

    virtual void InitResource(bool force = false) override;

    virtual void StatusUpdate() override;

    virtual void FinalUpdate() override;
};

