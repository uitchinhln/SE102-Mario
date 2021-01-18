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
	__event void PlayerBonusEvent(const char* source, shared_ptr<IEffect>& effect, Score score);

	static GameEvent* GetInstance();
};

