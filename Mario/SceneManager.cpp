#include "SceneManager.h"

SceneManager* SceneManager::__instance = nullptr;

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
