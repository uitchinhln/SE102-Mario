#include "HudPanel.h"
#include "SpriteManager.h"

void HudPanel::InitResource()
{
	if (sprites.size() < 1) {
		sprites["Default"] = SpriteManager::GetInstance()->Get("spr-hud-0");
	}
}

void HudPanel::Update()
{
}

void HudPanel::Render()
{
	InitResource();
	sprites["Default"]->Draw(Position.x + Size.x / 2, Position.y + Size.y / 2, trans);
}
