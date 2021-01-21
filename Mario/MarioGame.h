#pragma once
#include "Game.h";
#include "GameEnum.h"

class MainUI;
class PlayerData;
class Mario;
class TinyMario;
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
	shared_ptr<TinyMario> tinyMario;

	FightProcessor* fightProc;
	AchievementProcessor* achievementProc;

	GameState state = GameState::GAME_START;

public:
	MarioGame();

	virtual void LoadResources() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnKeyUp(int key) override;
	virtual void OnKeyDown(int key) override;

	virtual shared_ptr<PlayerData> GetPlayerData();

	virtual shared_ptr<Mario> GetMario();

	virtual shared_ptr<TinyMario> GetTinyMario();

	FightProcessor* GetFightProcessor();

	AchievementProcessor* GetAchievementProcessor();

	GameState GetGameState();

	MainUI* GetMainUI();

	void SetGameState(GameState value);

	static MarioGame* GetInstance();
};

