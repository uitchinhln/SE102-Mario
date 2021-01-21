#include "MainUI.h"
#include "SceneManager.h"
#include "MarioGame.h"
#include "GameEvent.h"
#include "RectangleInFX.h"
#include "FadeFX.h"
#include "PlaySceneFinishFX.h"

void MainUI::GameStartUpdate()
{
	MarioGame::GetInstance()->SetGameState(GameState::GAME_INTRO);
}

void MainUI::GameIntroUpdate()
{
	SceneManager::GetInstance()->Update();
}

void MainUI::GameMenuUpdate()
{
}

void MainUI::GameWorldMapUpdate()
{
	MarioGame::GetInstance()->GetPlayerData()->RemainingTime = 0;
	SceneManager::GetInstance()->Update();
}

void MainUI::GamePlayUpdate(GameState state)
{
	long dt = MarioGame::Time().ElapsedGameTime;
	shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();

	if (state == GameState::WORLD_START) {
		if (!timer.IsRunning()) {
			SceneManager::GetInstance()->Update();
			timer.Start();
		}
		else if (timer.Elapsed() > 300) {
			MarioGame::GetInstance()->SetGameState(GameState::WORLD_RUN);
			timer.Stop();
			timer.Reset();
		}
	} else if(state == GameState::WORLD_RUN) {

		data->RemainingTime -= dt;
		SceneManager::GetInstance()->Update();

	}else if(state == GameState::WORLD_LOSE) {
		if (!timer.IsRunning()) {
			timer.Start();
		}
		else if (timer.Elapsed() > 2500 && step == 0) {
			step = 1;
			fullViewEffect->SpawnEffect(make_shared<FadeFX>(fullView->GetScissorRect(), 1000, [](long playTime) {
				SceneManager::GetInstance()->ActiveScene("overworld");
				MarioGame::GetInstance()->SetGameState(GameState::GAME_WORLDMAP);
				MarioGame::GetInstance()->GetPlayerData()->Power = MEntityType::SmallMario;
				MarioGame::GetInstance()->GetPlayerData()->Lives -= 1;
				}));
		}
		else {
			SceneManager::GetInstance()->Update();
		}

	}else if(state == GameState::WORLD_WIN) {
		SceneManager::GetInstance()->Update();
		if (step == 0) {
			playsceneFinishFX = make_shared<PlaySceneFinishFX>(Vec2(0, 0), 15000, reward);
			fullViewEffect->SpawnEffect(playsceneFinishFX);
			step = 1;
		} else if (step == 1) {
			if (!timer.IsRunning()) timer.Start();
			if (timer.Elapsed() > 700) {
				data->Cards.push_back(reward);
				step = 2;
				timer.Stop();
				timer.Reset();
			}
		}
		else if (step == 2) {
			int delta = CGame::Time().ElapsedGameTime / 8;

			if (data->RemainingTime >= delta * 1000) {
				data->RemainingTime -= 1000 * delta;
				data->Score += 50 * delta;
			}
			else {
				data->Score += 50 * (data->RemainingTime / 1000);
				data->RemainingTime = 0;

				timer.Reset();
				timer.Start();
				step = 3;
			}
		}
		else if (step == 3) {
			if (timer.Elapsed() > 1000) {
				if (playsceneFinishFX != nullptr) {
					playsceneFinishFX->Active = false;
					playsceneFinishFX.reset();
				}
				timer.Stop();
				timer.Reset();

				step = 4;
			}
		}
		else if (step == 4) {
			fullViewEffect->SpawnEffect(make_shared<FadeFX>(fullView->GetScissorRect(), 1000, [](long playTime) {
				SceneManager::GetInstance()->ActiveScene("overworld");
				MarioGame::GetInstance()->SetGameState(GameState::GAME_WORLDMAP);
				}));
			step = 5;
		}
	}
}

void MainUI::GameOverUpdate()
{
}