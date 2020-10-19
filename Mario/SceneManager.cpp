#include "SceneManager.h"

SceneManager* SceneManager::__instance = nullptr;

void SceneManager::AddScene(string id, shared_ptr<CScene> scene)
{
	if (!scene) return;
	scenes[id] = scene;
}

bool SceneManager::ActiveScene(string id)
{
	if (scenes.find(id) == scenes.end()) return false;

	activeSceneID = id;
	GetActiveScene()->Load();
	return true;
}

shared_ptr<CScene> SceneManager::GetActiveScene()
{
	if (scenes.find(activeSceneID) == scenes.end()) return nullptr;
	return scenes.at(activeSceneID);
}

void SceneManager::Update()
{
	GetActiveScene()->Update();
}

void SceneManager::Render()
{
	GetActiveScene()->Render();
}

SceneManager* SceneManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new SceneManager();
	}
	return __instance;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}
