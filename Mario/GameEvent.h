#pragma once
#include "Utils.h"
#include "MarioUtils.h"
#include "IEffect.h"

[event_source(native)]
class GameEvent
{
	static GameEvent* __instance;
	GameEvent();

public:
	__event void PlayerScoreEvent(const char* source, shared_ptr<IEffect>& effect, Score score);
	__event void PlayerCoinEvent(const char* source, int value);

	static GameEvent* GetInstance();
};

