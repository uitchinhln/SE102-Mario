#pragma once
#include "Camera.h"
#include "GameMap.h"
#include "GameObject.h"
#include "MapProperties.h"
class CScene
{
protected:
	string id;
	string dataPath = "";

	shared_ptr<Camera> camera;
	shared_ptr<CGameMap> gameMap;

	unordered_map<DWORD, shared_ptr<GameObject>> objects;

public:
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update();
	virtual void Render();

	virtual void SpawnEntity(shared_ptr<GameObject> entity);

	virtual void SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props);

	virtual void DespawnEntity(shared_ptr<GameObject> entity);

	virtual unordered_map<DWORD, shared_ptr<GameObject>> GetObjects();

	virtual vector<shared_ptr<GameObject>> GetStaticObjects();

	virtual vector<shared_ptr<GameObject>> GetMovingObjects();

	virtual void OnKeyDown(int key);
	virtual void OnKeyUp(int key);

	virtual shared_ptr<Camera> GetCamera();
	virtual shared_ptr<CGameMap> GetGameMap();

	virtual void SetSceneContentPath(string path);

protected:
	virtual void RemoveDespawnedObjects();
};

typedef CScene* Scene;

