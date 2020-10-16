#pragma once
#include "Camera.h"
#include "GameMap.h"
#include "GameObject.h"
class CScene
{
protected:
	string id;

	Camera* camera;
	GameMap gameMap;

	vector<GameObject> objects;

public:
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update(int dt);
	virtual void Render();
};

typedef CScene* Scene;

