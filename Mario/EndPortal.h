#pragma once
#include "Portal.h"
#include "MapProperties.h"

class EndPortal :
    public Portal
{
public:
    virtual void Update() override;

    virtual ObjectType GetObjectType() override;

    static shared_ptr<EndPortal> CreatePortal(Vec2 fixedPos, Vec2 size, MapProperties properties);
protected:
    Direction direction;
};

