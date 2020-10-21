#include "Scene.h"

void CScene::Unload()
{
	objects.clear();
}

void CScene::Update()
{
	gameMap->Update();
	camera->Update();
	
	for (shared_ptr<CGameObject> obj : objects) {
		obj->PhysicUpdate();
		obj->Update(&gameMap->GetColliableTileAround(obj->GetPosition(), obj->GetHitBox(), obj->GetDistance()));
	}
}

void CScene::Render()
{
	gameMap->Render();

	for each (shared_ptr<CGameObject> obj in objects)
	{
		obj->Render();
	}
}

shared_ptr<Camera> CScene::GetCamera()
{
	return camera;
}

shared_ptr<CGameMap> CScene::GetGameMap()
{
	return gameMap;
}
