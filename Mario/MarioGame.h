#pragma once
#include "Game.h";
#include "GameEnum.h"

class MainUI;
class PlayerData;
class Mario;
class FightProcessor;
class AchievementProcessor;

class MarioGame : public CGame
{
private:
	static MarioGame* __instance;

protected:
	virtual void LoadDefaultFont();

	MainUI* main;

	shared_ptr<PlayerData> playerdata;

	shared_ptr<Mario> mario;

	FightProcessor* fightProc;
	AchievementProcessor* achievementProc;

	GameState state = GameState::CREATED;

public:
	MarioGame();

	virtual void LoadResources() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual shared_ptr<PlayerData> GetPlayerData();

	virtual shared_ptr<Mario> GetMario();

	FightProcessor* GetFightProcessor();

	AchievementProcessor* GetAchievementProcessor();

	GameState GetGameState();

	void SetGameState(GameState value);

	static MarioGame* GetInstance();
};

