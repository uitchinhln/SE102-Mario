#include "Hud.h"
#include "Game.h"

Hud::Hud(Vec2 pos, Vec2 size) : Viewport(pos, size)
{
}

void Hud::Update()
{
}

void Hud::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));
}
