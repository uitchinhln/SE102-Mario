#include "Scene.h"

void CScene::Unload()
{
	objects.clear();
}

void CScene::Update()
{
	gameMap->Update();
	camera->Update();

	vector<shared_ptr<IColliable>> objs;

	for (shared_ptr<GameObject> obj : objects) {
		objs.clear();
		objs.insert(objs.end(), objects.begin(), objects.end());

		vector<shared_ptr<IColliable>> tiles = gameMap->GetColliableTileAround(obj->GetPosition(), obj->GetHitBox(), obj->GetDistance());
		objs.insert(objs.end(), tiles.begin(), tiles.end());
		
		obj->Update(&objs);
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

void CScene::SpawnEntity(shared_ptr<GameObject> entity)
{
	objects.push_back(entity);
	entity->GetDataTag()->RemoveTag("despawned");
}

void CScene::DespawnEntity(shared_ptr<GameObject> entity)
{
	entity->GetDataTag()->SetBool("despawned", true);
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

void CScene::RemoveDespawnedObjects()
{
	objects.erase(remove_if(objects.begin(), objects.end(), [](const shared_ptr<GameObject>& obj) {
		return obj->GetDataTag()->HasKey("despawned");
		}), objects.end());
}
