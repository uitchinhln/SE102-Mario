#pragma once
#include "Utils.h"
#include "ColliableTile.h"

[event_source(native)]
class Events
{
	static Events* __instance;
	Events();

public:
	__event void KeyUpEvent(int keyCode);
	__event void KeyDownEvent(int keyCode);
	__event void ColliableTilePreLoadEvent(const char* type, int id, shared_ptr<ColliableTile>& tile);
	__event void ObjectLoadEvent(const char* type, Vec2 fixedPos);

	static Events* GetInstance();
	~Events();
};

