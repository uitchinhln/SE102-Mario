#pragma once
#include <vector>
#include "EventHandler.h"

using namespace std;

class EventManager
{
private:
	vector<EventHandler*> handlers;
	
	static EventManager* Instance;

	EventManager();

public:

	static EventManager* GetInstance();

	static EventManager* StartService(bool force = false);
};

