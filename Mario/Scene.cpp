#include "Scene.h"

void CScene::Unload()
{
	objects.clear();
}

void CScene::Update()
{

}

void CScene::Render()
{
	//auto start = std::chrono::high_resolution_clock::now();

	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Loop: %d\t%d\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

void CScene::SpawnEntity(shared_ptr<GameObject> entity)
{
	objects[entity->GetID()] = entity;
	entity->SetActive(true);
}

void CScene::SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props)
{
	objects[entity->GetID()] = entity;
	entity->SetActive(true);
}

void CScene::DespawnEntity(shared_ptr<GameObject> entity)
{
	entity->SetActive(false);
}

unordered_map<DWORD, shared_ptr<GameObject>> CScene::GetObjects()
{
	return objects;
}

vector<shared_ptr<GameObject>> CScene::GetStaticObjects()
{
	return vector<shared_ptr<GameObject>>();
}

vector<shared_ptr<GameObject>> CScene::GetMovingObjects()
{
	return vector<shared_ptr<GameObject>>();
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

void CScene::SetSceneContentPath(string path)
{
	TiXmlDocument doc(path.c_str());

	if (!doc.LoadFile()) {
		DebugOut(L"Get scene data failed. File: %s\n", ToLPCWSTR(path));
		return;
	}
	DebugOut(L"Get scene data success. File: %s\n", ToLPCWSTR(path));
	doc.Clear();

	this->dataPath = path;
}

void CScene::RemoveDespawnedObjects()
{
	//objects.erase(remove_if(objects.begin(), objects.end(), [](const shared_ptr<GameObject>& obj) {
	//	return !obj->IsActive();
	//	}), objects.end());
	for (auto it = objects.begin(); it != objects.end(); ) {
		if (!(*it).second->IsActive()) 
			it = objects.erase(it);
		else 
			++it;
	}
}
