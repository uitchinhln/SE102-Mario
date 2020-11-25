#include "Hud.h"
#include "Game.h"
#include "FontManager.h"

Hud::Hud(Vec2 pos, Vec2 size) : Viewport(pos, size)
{
}

void Hud::Update()
{
}

void Hud::Render()
{
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(181, 235, 242));

	//RECT FontRect = scissorRect;
	////FontRect.left = 10;         // xPos
	//FontRect.top += 10;         // yPos
	////FontRect.right = 500;        // maxX Cutoff
	////FontRect.bottom = 130;        // maxY Cutoff


	//FontManager::GetInstance()->Get("Test")->DrawTextA(
	//	NULL,                                  // Sprite
	//	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789 the quick brown fox jupms over the lazy dog",                           // Text
	//	strlen("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789 the quick brown fox jupms over the lazy dog"),                   // Text length
	//	&FontRect,                             // The font RECT
	//	DT_LEFT | DT_TOP | DT_WORDBREAK,                      // Format
	//	D3DCOLOR_XRGB(0, 0, 0));
}
