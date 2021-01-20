#pragma once
#include "Utils.h"
#include "GameEnum.h"

class PlayerData;

class AchievementProcessor
{
protected:
	shared_ptr<PlayerData> data;

public:
	AchievementProcessor(shared_ptr<PlayerData> data);

	void OnPlayerCoinChange(const char* file, int amount);
	void OnPlayerScoreChange(const char* file, Score score, Vec2 Position, ScoreSource reason);
	void OnPlayerLifeChange(const char* file, Vec2 Position);
};

