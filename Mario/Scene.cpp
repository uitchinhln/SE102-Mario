#include "Scene.h"

void CScene::Unload()
{
	if (camera) {
		delete camera;
		camera = nullptr;
	}
	if (gameMap) {
		delete gameMap;
		gameMap = nullptr;
	}

	for (GameObject obj : objects) {
		if (obj) delete obj;
	}
	objects.clear();
}

void CScene::Update(int dt)
{
	gameMap->Update(dt);
	
	for (GameObject obj : objects) {
		obj->Update(dt);
	}
}

void CScene::Render()
{
	gameMap->Render();

	for each (GameObject obj in objects)
	{
		obj->Render();
	}
}
