#include "FontManager.h"
#include "Game.h"

FontManager* FontManager::__instance = NULL;

FontManager* FontManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new FontManager();
	}
	return __instance;
}

LPD3DXFONT FontManager::Get(string id)
{
	if (fonts.find(id) == fonts.end()) return nullptr;
	return fonts[id];
}

void FontManager::HardTest()
{
	LPD3DXFONT ArialFont;
	D3DXCreateFont(
		CGame::GetInstance()->GetGraphic().GetDirect3DDevice(), // The device
		30,                        // Font size
		0,                         // Default width
		FW_NORMAL,                 // Font weight
		2,                         // MipMap
		false,                     // Italic
		DEFAULT_CHARSET,           // Charset
		OUT_DEFAULT_PRECIS,        // Output precision
		DEFAULT_QUALITY,           // Quality
		DT_LEFT | DT_TOP,          // Default pitch & family
		L"Super Mario Bros. 3",                   // Font name
		&ArialFont);
	fonts["Test"] = ArialFont;
}

FontManager::FontManager() {

}