#include "EventManager.h"

EventManager* EventManager::GetInstance()
{
	return Instance;
}

EventManager* EventManager::StartService(bool force = false)
{
	if (Instance == nullptr || force) {
		delete Instance;
		Instance = new EventManager();
	}
	return Instance;
}
