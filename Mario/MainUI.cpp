#include "MainUI.h"
#include "SceneManager.h"
#include "MarioGame.h"
#include "GameEvent.h"
#include "RectangleInFX.h"
#include "FadeFX.h"

MainUI::MainUI()
{
	worldView = make_shared<Viewport>(Vec2(0, 0), Vec2(769, 579));
	fullView = make_shared<Viewport>(Vec2(0, 0), Vec2(769, 721));

	fullViewEffect = new EffectServer();
	worldViewEffect = new EffectServer();
	hudViewEffect = new EffectServer();

	timer.Stop();
	timer.Reset();

	__hook(&GameEvent::PlaySceneEndEvent, GameEvent::GetInstance(), &MainUI::OnPlaySceneEnd);
}

void MainUI::Update()
{
	GameState state = MarioGame::GetInstance()->GetGameState();

	switch (state)
	{
	case GameState::GAME_START:
		GameStartUpdate();
		break;
	case GameState::GAME_INTRO:
		GameIntroUpdate();
		break;
	case GameState::GAME_MENU:
		GameMenuUpdate();
		break;
	case GameState::GAME_WORLDMAP:
		GameWorldMapUpdate();
		break;
	case GameState::GAME_OVER:
		GameOverUpdate();
		break;
	case GameState::WORLD_START:
	case GameState::WORLD_RUN:
	case GameState::WORLD_LOSE:
	case GameState::WORLD_WIN:
		GamePlayUpdate(state);
		break;
	}

	if (dialog != nullptr) {
		dialog->Update();
	}

	hud->Update();
	worldViewEffect->Update();
	hudViewEffect->Update();
	fullViewEffect->Update();
}

void MainUI::Render()
{
	GameState state = MarioGame::GetInstance()->GetGameState();

	switch (state)
	{
	case GameState::GAME_START:
		GameStartRender();
		break;
	case GameState::GAME_INTRO:
		GameIntroRender();
		break;
	case GameState::GAME_MENU:
		GameMenuRender();
		break;
	case GameState::GAME_WORLDMAP:
		GameWorldMapRender();
		break;
	case GameState::GAME_OVER:
		GameOverRender();
		break;
	case GameState::WORLD_START:
	case GameState::WORLD_RUN:
	case GameState::WORLD_LOSE:
	case GameState::WORLD_WIN:
		GamePlayRender(state);
		break;
	}

	if (dialog != nullptr) {
		dialog->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void MainUI::CreateHUD(TiXmlElement* node)
{
	Vec2 pos;
	string path = node->Attribute("path");

	node->QueryFloatAttribute("left", &pos.x);
	node->QueryFloatAttribute("top", &pos.y);

	hud = make_shared<Hud>(path, pos, Vec2(769, 142));
}

void MainUI::ActiveDialog(shared_ptr<Dialog> dialog)
{
	this->dialog = dialog;
}

void MainUI::DeactiveDialog()
{
	this->dialog.reset();
}

void MainUI::OnSceneChange(const char* file, SceneType type)
{
	switch (type)
	{
	case SceneType::INTRO:
		break;
	case SceneType::MENU:
		break;
	case SceneType::WORLDMAP:
		fullViewEffect->SpawnEffect(make_shared<FadeFX>(fullView->GetScissorRect(), 1000, [](long playTime) {
			SceneManager::GetInstance()->ActiveScene("overworld");
			MarioGame::GetInstance()->SetGameState(GameState::GAME_WORLDMAP);
			}));
		break;
	case SceneType::PLAYSCENE:
		break;
	case SceneType::GAMEOVER:
		break;
	default:
		break;
	}
}

void MainUI::OnGameStateChange(const char* file, GameState current, GameState next)
{
	step = 0;
	timer.Stop();
	timer.Reset();
}

void MainUI::OnPrePlaySceneBegin(int nodeId, const char* sceneId, int worldId)
{
	worldViewEffect->SpawnEffect(make_shared<RectangleInFX>(worldView->GetScissorRect(), 500, Vec2(24, 24), [nodeId, sceneId, worldId](long playedTime) {
		__raise (*GameEvent::GetInstance()).PlaySceneBeginEvent(__FILE__, nodeId, sceneId, worldId);
		SceneManager::GetInstance()->ActiveScene(sceneId);
		MarioGame::GetInstance()->SetGameState(GameState::WORLD_START);
		MarioGame::GetInstance()->GetPlayerData()->World = worldId;
		}));
	hudViewEffect->SpawnEffect(make_shared<FadeFX>(worldView->GetScissorRect(), 800, [nodeId, sceneId, worldId](long playedTime) {
		}));
}

void MainUI::OnPlaySceneEnd(const char* file, SceneResult result, CardType reward)
{
	if (result == SceneResult::WIN) {
		this->reward = reward;
		MarioGame::GetInstance()->SetGameState(GameState::WORLD_WIN);
	}
	else {
		MarioGame::GetInstance()->SetGameState(GameState::WORLD_LOSE);
	}
}

void MainUI::OnKeyUp(int keycode)
{
	if (dialog != nullptr) {
		dialog->OnKeyUp(keycode);
	}
	else {
		SceneManager::GetInstance()->OnkeyUp(keycode);
	}
}

void MainUI::OnKeyDown(int keycode)
{
	if (dialog != nullptr) {
		dialog->OnKeyDown(keycode);
	}
	else {
		SceneManager::GetInstance()->OnKeyDown(keycode);
	}
}
