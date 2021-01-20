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

	bool pauseGameUpdate = false;
	bool pauseGameRender = false;

	shared_ptr<Camera> camera;
	shared_ptr<CGameMap> gameMap;

	unordered_map<DWORD, shared_ptr<GameObject>> objects;
	vector<shared_ptr<GameObject>> objectsWithoutGrid;

public:
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update();
	virtual void Render();

	virtual void SpawnEntity(shared_ptr<GameObject> entity);

	virtual void SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props);

	virtual void SpawnEntityWithoutGrid(shared_ptr<GameObject> entity);

	virtual void DespawnEntity(shared_ptr<GameObject> entity);

	virtual unordered_map<DWORD, shared_ptr<GameObject>> GetObjects();

	virtual vector<shared_ptr<GameObject>> GetUpdatingObjects();

	virtual void OnKeyDown(int key);
	virtual void OnKeyUp(int key);

	virtual bool IsGameUpdatePaused();
	virtual void SetGamePauseUpdate(bool value);

	virtual bool IsGameRenderPaused();
	virtual void SetPauseGameRender(bool value);

	virtual shared_ptr<Camera> GetCamera();
	virtual shared_ptr<CGameMap> GetGameMap();

	virtual void SetSceneContentPath(string path);

protected:
	virtual void RemoveDespawnedObjects();
};

typedef CScene* Scene;

