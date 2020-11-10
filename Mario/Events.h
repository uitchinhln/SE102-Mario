#pragma once
#include "Utils.h"
#include "ColliableTile.h"
#include "MapProperties.h"

[event_source(native)]
class Events
{
	static Events* __instance;
	Events();

public:
	__event void KeyUpEvent(int keyCode);
	__event void KeyDownEvent(int keyCode);
	__event void ColliableTilePreLoadEvent(const char* type, int id, shared_ptr<ColliableTile>& tile, MapProperties& properties);
	__event void MapBlockPreLoadEvent(int id, ObjectType& type, Vec2 pos, bool& cancel);
	__event void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& properties);

	static Events* GetInstance();
	~Events();
};

