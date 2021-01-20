#pragma once
#include "UIComponents.h"
#include "PlayerData.h"

class CardSlot :
    public UIComponents
{
protected:
    Transform trans;
    CardType cardType;

    long flickDelay;

    long flickDuration;

public:
    CardSlot(CardType type);

    virtual void SetCard(CardType type);

    virtual void SetCard(CardType type, long flickDuration, long delay);

    virtual void SetAndAutoFlick(CardType type, long flickDuration, long delay);

    virtual void InitResource() override;

    virtual void Update() override;

    virtual void Render(D3DCOLOR overlay) override;
};

