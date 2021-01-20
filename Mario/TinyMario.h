#pragma once
#include "GameObject.h"
#include "PlayerData.h"
#include "Graph.h"
#include "MEntityType.h"

[event_receiver(native)]
class TinyMario :
    public GameObject,
    public enable_shared_from_this<TinyMario>
{
protected:
    shared_ptr<PlayerData> data;

    shared_ptr<Graph> graph;

    Vec2 size;

    int currentStation = 0;

    int moveStep = 0;

    int targetStation = 0;

    Vec2 targetPosition = VECTOR_0;

    ObjectType currentPower = MEntityType::SmallMario;
    bool resourceChange = false;

public:
    TinyMario();

    virtual void InitResource();


    virtual void Update() override;

    virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

    virtual void PositionUpdate() override;

    virtual void RestoreCollision() override;

    virtual void PositionLateUpdate() override;

    virtual void CollisionDoubleFilter() override;

    virtual void StatusUpdate() override;

    virtual void FinalUpdate() override;

    virtual bool HasCollideWith(DWORD id) override;

    virtual void Render() override;

    virtual RectF GetHitBox() override;

    virtual bool IsGetThrough(GameObject& object, Direction direction) override;

    virtual float GetDamageFor(GameObject& object, Direction direction) override;

    virtual ObjectType GetObjectType() override;


    virtual shared_ptr<Graph> GetGraph();

    virtual void AddNode(shared_ptr<MapGate> node);

    virtual shared_ptr<PlayerData> GetPlayerData();


    virtual void HookEvent();

    virtual void UnhookEvent();

    virtual void OnKeyDown(int key);

    virtual void OnPlaySceneFinish(const char* source, CardType reward);
};

