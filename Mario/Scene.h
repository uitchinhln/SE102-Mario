#pragma once
#include "Camera.h"
#include "GameMap.h"
#include "GameObject.h"
class CScene
{
protected:
	string id;

	shared_ptr<Camera> camera;
	shared_ptr<CGameMap> gameMap;

	vector<shared_ptr<GameObject>> objects;

public:
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update();
	virtual void Render();

	virtual void SpawnEntity(shared_ptr<GameObject> entity);

	virtual void DespawnEntity(shared_ptr<GameObject> entity);

	virtual vector<shared_ptr<GameObject>> GetObjects();

	virtual void OnKeyDown(int key);
	virtual void OnKeyUp(int key);

	virtual shared_ptr<Camera> GetCamera();
	virtual shared_ptr<CGameMap> GetGameMap();

protected:
	virtual void RemoveDespawnedObjects();
};

typedef CScene* Scene;

