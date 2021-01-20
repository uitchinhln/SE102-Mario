#pragma once
#include "GameObject.h"

class Mario;

class Portal :
    public GameObject
{
public:
    Portal();

    virtual void Update() override;

    virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

    virtual void PositionUpdate() override;

    virtual void RestoreCollision() override;

    virtual void PositionLateUpdate() override;

    virtual void CollisionDoubleFilter() override;

    virtual void StatusUpdate() override;

    virtual void FinalUpdate() override;

    virtual bool HasCollideWith(DWORD id) override;

    virtual void Render(D3DCOLOR overlay) override;

    virtual RectF GetHitBox() override;

    virtual bool IsGetThrough(GameObject& object, Direction direction) override;

    virtual float GetDamageFor(GameObject& object, Direction direction) override;

protected:
    RectF hitbox;
    Vec2 size;

    bool hasMario = false;

    Direction direction = Direction::Bottom;
};

