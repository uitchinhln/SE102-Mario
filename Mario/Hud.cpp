#include "Hud.h"
#include "Game.h"

Hud::Hud(Vec2 pos, Vec2 size)
{
	this->pos = pos;
	this->size = size;

	vp.X = pos.x;
	vp.Y = pos.y;
	vp.Width = size.x + 1;
	vp.Height = size.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
}

void Hud::Update()
{
}

void Hud::Render()
{
	CGame::GetInstance()->GetGraphic().ClipScene();
	CGame::GetInstance()->GetGraphic().GetDirect3DDevice()->GetViewport(&oldVP);
	CGame::GetInstance()->GetGraphic().GetDirect3DDevice()->SetViewport(&vp);
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(255, 255, 255));
	CGame::GetInstance()->GetGraphic().GetDirect3DDevice()->SetViewport(&oldVP);
}
