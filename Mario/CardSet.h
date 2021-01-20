#pragma once
#include "UIComponents.h"
#include "CardSlot.h"
#include "PlayerData.h"

class CardSet :
    public UIComponents
{
protected:
    vector<CardSlot*> cards;
    shared_ptr<PlayerData> data;

public:

    virtual void Init();

    virtual void Add(Vec2 pos, Vec2 size);

    virtual void Update() override;

    virtual void Render() override;

    ~CardSet();
};

