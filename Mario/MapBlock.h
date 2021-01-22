#pragma once
#include "GameObject.h"
class MapBlock :
    public GameObject
{
public:
	MapBlock();

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
};

