#pragma once
#include "libs.h"

class EventManager
{
private:
	//vector<EventHandler*> handlers;
	
	static EventManager* __instance;

	EventManager() {};

public:

	static EventManager* GetInstance();

	static EventManager* StartService(bool force);
};

