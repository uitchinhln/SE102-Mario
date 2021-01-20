#pragma once
#include "UIComponents.h"
#include "PlayerData.h"

class Mario;

class PMeter :
    public UIComponents
{
protected:
    Transform trans;

    int level;

public:
    PMeter();

    virtual void InitResource() override;

    virtual void Update() override;

    virtual void Render() override;
};

