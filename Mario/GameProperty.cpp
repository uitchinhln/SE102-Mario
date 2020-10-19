#include "GameProperty.h"

CGameProperties::CGameProperties()
{
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.BackBufferHeight = ScreenHeight + 1;
	d3dpp.BackBufferWidth = ScreenWidth + 1;
}
