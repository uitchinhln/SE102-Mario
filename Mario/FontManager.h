#pragma once
#include "Utils.h"

class FontManager
{
public:
	static FontManager* GetInstance();

	//void LoatCustomFont(string path);

	LPD3DXFONT Get(string id);

	void HardTest();

private:
	static FontManager* __instance;

	unordered_map<string, LPD3DXFONT> fonts;

	FontManager();
};

