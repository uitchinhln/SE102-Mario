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

	if (GetActiveScene() != nullptr) {
		GetActiveScene()->Unload();
	}
	
	activeSceneID = id;
	auto start = std::chrono::high_resolution_clock::now();
	GetActiveScene()->Load();
	auto finish = std::chrono::high_resolution_clock::now();
	DebugOut(L"Loop: %d\t%d\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
	return true;
}

shared_ptr<CScene> SceneManager::GetActiveScene()
{
	if (scenes.find(activeSceneID) == scenes.end()) return nullptr;
	return scenes.at(activeSceneID);
}

void SceneManager::SetPlayer(shared_ptr<GameObject> player)
{
	this->player = player;
}

void SceneManager::Update()
{
	GetActiveScene()->Update();
}

void SceneManager::Render()
{
	GetActiveScene()->Render();
}

void SceneManager::OnKeyDown(int key)
{
	shared_ptr<CScene> s = GetActiveScene();
	if (s != NULL) {
		s->OnKeyDown(key);
	}
}

void SceneManager::OnkeyUp(int key)
{
	shared_ptr<CScene> s = GetActiveScene();
	if (s != NULL) {
		s->OnKeyUp(key);
	}
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
