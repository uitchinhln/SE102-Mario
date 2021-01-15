#pragma once
#include "Utils.h"
#include "MapProperties.h"

[event_source(native)]
class Events
{
	static Events* __instance;
	Events();

public:
	__event void KeyUpEvent(int keyCode);
	__event void KeyDownEvent(int keyCode);
	__event void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& properties);
	__event void MapReadEvent(MapProperties& properties);

	static Events* GetInstance();
	~Events();
};

