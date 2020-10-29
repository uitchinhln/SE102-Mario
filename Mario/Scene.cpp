#include "Scene.h"

void CScene::Unload()
{
	objects.clear();
}

void CScene::Update()
{
	gameMap->Update();
	camera->Update();
	
	for (shared_ptr<GameObject> obj : objects) {
		vector<shared_ptr<IColliable>> objs;
		obj->Update(&gameMap->GetColliableTileAround(obj->GetPosition(), obj->GetHitBox(), obj->GetDistance()));
	}
}

void CScene::Render()
{
	gameMap->Render();

	for each (shared_ptr<GameObject> obj in objects)
	{
		obj->Render();
	}
}

void CScene::OnKeyDown(int key)
{
}

void CScene::OnKeyUp(int key)
{
}

shared_ptr<Camera> CScene::GetCamera()
{
	return camera;
}

shared_ptr<CGameMap> CScene::GetGameMap()
{
	return gameMap;
}
