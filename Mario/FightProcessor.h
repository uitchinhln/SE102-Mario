#pragma once
#include "Utils.h"
#include "ObjectType.h"
#include "GameEnum.h"

class PlayerData;

[event_receiver(native)]
class FightProcessor
{
protected:
	shared_ptr<PlayerData> data;

	long last = 0;

	Score lastScore;

	bool enable = true;

public:
	FightProcessor(shared_ptr<PlayerData> data);

	void SetEnable(bool value);

	bool IsEnable();

	void OnEnemyDamaged(const char* file, DamgeSource source, Vec2 Position, ObjectType& victimType);

	void OnPlaySceneClear(const char* file, SceneResult result);
};

