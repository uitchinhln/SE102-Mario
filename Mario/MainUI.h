#pragma once
#include "Camera.h"
#include "Hud.h"
#include "GameEnum.h"
#include "EffectServer.h"

class PlaySceneFinishFX;

class MainUI
{
public:
	MainUI();

	virtual void Update();
	virtual void Render();

	void GameStartUpdate();
	void GameIntroUpdate();
	void GameMenuUpdate();
	void GameWorldMapUpdate();
	void GamePlayUpdate(GameState);
	void GameOverUpdate();

	void GameStartRender();
	void GameIntroRender();
	void GameMenuRender();
	void GameWorldMapRender();
	void GamePlayRender(GameState);
	void GameOverRender();

	virtual void CreateHUD(TiXmlElement* node);

	void OnSceneChange(const char* file, SceneType type);

	void OnGameStateChange(const char* file, GameState current, GameState next);

	void OnPrePlaySceneBegin(int nodeId, const char* sceneId, int worldId);

	void OnPlaySceneEnd(const char* file, SceneResult result, CardType reward);

	void OnKeyUp(int keycode);

	void OnKeyDown(int keycode);

	int step = 0;

protected:
	Stopwatch timer;

	shared_ptr<Hud> hud;

	shared_ptr<Viewport> fullView;
	shared_ptr<Viewport> worldView;

	EffectServer* fullViewEffect;
	EffectServer* hudViewEffect;
	EffectServer* worldViewEffect;


	CardType reward = CardType::EMPTY;
	shared_ptr<PlaySceneFinishFX> playsceneFinishFX;
};

