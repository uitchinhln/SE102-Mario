#include "MenuDialog.h"
#include <dinput.h>
#include "SpriteManager.h"
#include "MarioGame.h"
#include "MainUI.h"

MenuDialog::MenuDialog()
{
	Position = Vec2(228, 410);
	Size = Vec2(312, 120);
}

void MenuDialog::Update()
{
}

void MenuDialog::Render(D3DCOLOR overlay)
{
	SpriteManager::GetInstance()->Get("spr-options-0")->Draw(Position.x + Size.x /2, Position.y + Size.y / 2, trans, overlay);
	if (state == MenuDialogState::FIRST) {
		SpriteManager::GetInstance()->Get("spr-menu-arrow-0")->Draw(Position.x + posArrow1.x + 11, Position.y + posArrow1.y + 12, trans, overlay);
	}
	else {
		SpriteManager::GetInstance()->Get("spr-menu-arrow-0")->Draw(Position.x + posArrow2.x + 11, Position.y + posArrow2.y + 12, trans, overlay);
	}
}

void MenuDialog::OnKeyUp(int key)
{
}

void MenuDialog::OnKeyDown(int key)
{
	if (key == DIK_UP || key == DIK_DOWN) {
		if (state == MenuDialogState::FIRST) {
			state = MenuDialogState::SECOND;
		}
		else {
			state = MenuDialogState::FIRST;
		}
	}
	if (key == DIK_RETURN) {
		MarioGame::GetInstance()->GetMainUI()->OnSceneChange(__FILE__, SceneType::WORLDMAP);
		MarioGame::GetInstance()->GetMainUI()->DeactiveDialog();
	}
}
