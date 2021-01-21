#pragma once
#include "Portal.h"
#include "MapProperties.h"

class BeginPortal :
    public Portal
{
public:
    virtual void Update() override;

    virtual ObjectType GetObjectType() override;

    static shared_ptr<BeginPortal> CreatePortal(Vec2 fixedPos, Vec2 size, MapProperties properties);

protected:
    int cameraRegionId = 0;
    Vec2 destination;
    Direction direction;
};

