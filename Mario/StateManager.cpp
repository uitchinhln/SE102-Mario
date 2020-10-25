#include "StateManager.h"

StateManager* StateManager::__instance = nullptr;

StateManager* StateManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new StateManager();
		__instance->contents.clear();
	}
	return __instance;
}

void StateManager::Add(string id, shared_ptr<MarioPowerUp> marioState)
{
	contents[id] = marioState;
}

shared_ptr<MarioPowerUp> StateManager::Get(string id)
{
	if (contents.find(id) == contents.end()) return NULL;
	return contents[id];
}

StateManager::StateManager()
{
}
