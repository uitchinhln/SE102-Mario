#pragma once
#include "MapBlock.h"
#include "MapProperties.h"


class MovingPlatform :
    public MapBlock
{
protected:
    Vec2 size;

public:
    MovingPlatform();

    virtual void InitResource();

    virtual void Update() override;

    virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

    virtual void PositionUpdate() override;

    virtual void StatusUpdate() override;

    virtual void Render(D3DCOLOR overlay) override;


    virtual ObjectType GetObjectType() override;

    virtual RectF GetHitBox() override;

    virtual bool IsGetThrough(GameObject& object, Direction direction) override;

    virtual float GetDamageFor(GameObject& object, Direction direction) override;

    static shared_ptr<MovingPlatform> CreateMovingPlatform(Vec2 pos, Vec2 size, MapProperties& props);
};

