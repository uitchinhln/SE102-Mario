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

	vector<shared_ptr<CGameObject>> objects;

public:
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update();
	virtual void Render();

	virtual shared_ptr<Camera> GetCamera();
	virtual shared_ptr<CGameMap> GetGameMap();
};

typedef CScene* Scene;

