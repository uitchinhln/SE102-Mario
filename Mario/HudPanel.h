#pragma once
#include "UIComponents.h"
class HudPanel :
    public UIComponents
{
protected:
    Transform trans;

public:
    virtual void InitResource() override;

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;
};

