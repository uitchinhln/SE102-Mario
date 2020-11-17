#pragma once
#include "Utils.h"

class FontManager
{
public:
	FontManager* GetInstance();

	void LoatCustomFont();

private:
	static FontManager* __instance;
	FontManager();
};

