#include "EventManager.h"

EventManager* EventManager::__instance = nullptr;

EventManager* EventManager::GetInstance()
{
	return __instance;
}

EventManager* EventManager::StartService(bool force)
{
	if (__instance == nullptr || force) {
		delete __instance;
		__instance = new EventManager();
	}
	return __instance;
}
