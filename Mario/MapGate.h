#pragma once
#include "Utils.h"
#include "SpriteManager.h"
#include "GameObject.h"
#include "MapProperties.h"

class PlayerData;

class MapGate : 
	public GameObject
{
protected:
	unordered_map<string, Sprite> sprites;

	int nodeId;

	string sceneID;

	unordered_map<Direction, int> adjacentNodes;

	Vec2 size;

	Transform trans;

	bool start = false;

	bool finish = false;

	int worldNumber = 0;

public:
	MapGate(int nodeId, string sceneID, string uncheck, string checked);

	virtual void InitResource();

	virtual void Reset();


	virtual void Update() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void RestoreCollision() override;

	virtual void PositionLateUpdate() override;

	virtual void CollisionDoubleFilter() override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Render(D3DXCOLOR overlay) override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	virtual ObjectType GetObjectType() override;


	virtual bool CanTravel(Direction direction, shared_ptr<PlayerData> data);

	virtual void Discover();


	virtual bool IsFinished();
	virtual void SetFinished(bool value);

	virtual int GetWorldNumber();

	virtual bool IsStart();

	virtual int GetNodeId();

	virtual string GetSceneID();

	unordered_map<Direction, int>& GetAdjacentList();

	static shared_ptr<MapGate> CreateMapGate(Vec2 pos, MapProperties props);
};

