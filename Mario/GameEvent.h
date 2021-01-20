#pragma once
#include "Utils.h"
#include "MarioUtils.h"

enum class CardType;
class IEffect;

[event_source(native)]
class GameEvent
{
	static GameEvent* __instance;
	GameEvent();

public:
	__event void PlayerScoreEvent(const char* source, shared_ptr<IEffect>& effect, Score score);
	__event void PlayerCoinEvent(const char* source, int value);
	__event void PlaySceneFinishEvent(const char* source, CardType reward);

	static GameEvent* GetInstance();
};

