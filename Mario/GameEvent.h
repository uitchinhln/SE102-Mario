#pragma once
#include "Utils.h"
#include "GameEnum.h"
#include "ObjectType.h"

enum class CardType;
class IEffect;

[event_source(native)]
class GameEvent
{
	static GameEvent* __instance;
	GameEvent();

public:
	__event void PlaySceneBeginEvent(const char* file, int nodeId, const char* sceneId, int worldId);
	__event void PlaySceneEndEvent(const char* file, SceneResult result, CardType reward);
	__event void PlaySceneClearEvent(const char* file, SceneResult result);

	__event void EnemyDamagedEvent(const char* file, DamgeSource source, Vec2 Position, ObjectType& victimType);

	__event void PlayerCoinChangeEvent(const char* file, int amount);
	__event void PlayerScoreChangeEvent(const char* file, Score score, Vec2 Position, ScoreSource reason);
	__event void PlayerLifeChangeEvent(const char* file, Vec2 Position);

	__event void SceneChangeEvent(const char* file, SceneType type);

	static GameEvent* GetInstance();
};

