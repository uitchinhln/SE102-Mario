#pragma once
#include "Utils.h"
#include "ObjectType.h"
#include "GameEnum.h"

class PlayerData;

class FightProcessor
{
protected:
	shared_ptr<PlayerData> data;

	long last = 0;

	Score lastScore;

public:
	FightProcessor(shared_ptr<PlayerData> data);

	void OnEnemyDamaged(const char* file, DamgeSource source, Vec2 Position, ObjectType& victimType);

	void OnPlaySceneEnd(const char* file, SceneResult result, CardType reward);
};

