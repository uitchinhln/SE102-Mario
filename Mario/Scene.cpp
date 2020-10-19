#include "Scene.h"

void CScene::Unload()
{
	objects.clear();
}

void CScene::Update()
{
	gameMap->Update();
	
	for (shared_ptr<CGameObject> obj : objects) {
		obj->Update(&gameMap->GetColliableTileAround(obj->GetPosition(), obj->GetDistance()));
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
