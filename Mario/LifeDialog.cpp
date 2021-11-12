#include "LifeDialog.h"
#include "MarioGame.h"
#include "PlayerData.h"
#include "MainUI.h"
#include "Text.h"
#include "SpriteManager.h"

LifeDialog::LifeDialog(long timeout)
{
	timer = timeout;

	Position = Vec2(192, 207);
	Size = Vec2(384, 165);

	life.SetFont(MarioGame::GetInstance()->DefaultFont);
	life.Position = Position + Vec2(280, 101);
	life.Size = Vec2(48, 24);

	world.SetFont(MarioGame::GetInstance()->DefaultFont);
	world.Position = Position + Vec2(215, 38);
	world.Size = Vec2(48, 24);
}

void LifeDialog::Update()
{
	shared_ptr<PlayerData> data = MarioGame::GetInstance()->GetPlayerData();
	life.SetContent(to_string(max(0, data->Lives)));
	world.SetContent(to_string(max(1, data->World)));

	timer -= MarioGame::Time().ElapsedGameTime;

	if (timer < 0) {
		MarioGame::GetInstance()->GetMainUI()->DeactiveDialog();
	}
}

void LifeDialog::Render(D3DXCOLOR overlay)
{
	SpriteManager::GetInstance()->Get("spr-dialog-mario-0")->Draw(Position.x + Size.x / 2, Position.y + Size.y / 2, trans, overlay);
	life.Render(overlay);
	world.Render(overlay);
}
