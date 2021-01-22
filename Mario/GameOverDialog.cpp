#include "GameOverDialog.h"
#include <dinput.h>
#include "MarioGame.h"
#include "MainUI.h"

GameOverDialog::GameOverDialog()
{
	Position = Vec2(192, 193);
	Size = Vec2(384, 192);
}

void GameOverDialog::Update()
{
}

void GameOverDialog::Render(D3DCOLOR overlay)
{
	SpriteManager::GetInstance()->Get("spr-dialog-gameover-0")->Draw(Position.x + Size.x / 2, Position.y + Size.y / 2, trans, overlay);
	if (state == GameOverDialogState::CONTINUE) {
		SpriteManager::GetInstance()->Get("spr-dialog-arrow-0")->Draw(Position.x + posArrow1.x + 9, Position.y + posArrow1.y + 11, trans, overlay);
	}
	else {
		SpriteManager::GetInstance()->Get("spr-dialog-arrow-0")->Draw(Position.x + posArrow2.x + 9, Position.y + posArrow2.y + 11, trans, overlay);
	}
}

void GameOverDialog::OnKeyUp(int key)
{
}

void GameOverDialog::OnKeyDown(int key)
{
	if (key == DIK_UP || key == DIK_DOWN) {
		if (state == GameOverDialogState::CONTINUE) {
			state = GameOverDialogState::END;
		}
		else {
			state = GameOverDialogState::CONTINUE;
		}
	}
	if (key == DIK_RETURN) {
		if (state == GameOverDialogState::CONTINUE) {
			MarioGame::GetInstance()->GetPlayerData()->Lives = 4;
			MarioGame::GetInstance()->GetMainUI()->OnSceneChange(__FILE__, SceneType::WORLDMAP);
			MarioGame::GetInstance()->GetMainUI()->DeactiveDialog();
		}
		else {
			MarioGame::GetInstance()->GetMainUI()->OnSceneChange(__FILE__, SceneType::INTRO);
			MarioGame::GetInstance()->GetMainUI()->DeactiveDialog();
		}
	}
}
