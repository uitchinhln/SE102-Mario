#pragma once
#include "Utils.h"

[event_source(native)]
class Events
{
	static Events* __instance;
	Events();

public:
	__event void KeyUpEvent(int keyCode);
	__event void KeyDownEvent(int keyCode);

	static Events* GetInstance();
	~Events();
};

