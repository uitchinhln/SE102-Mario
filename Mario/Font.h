#pragma once
#include "Utils.h"
#include "SpriteManager.h"

struct FontSprite
{
	char character;
	string spriteId;
};

class Font
{
public:
	Font();
	void Import(vector<FontSprite> fontSprites);
	Sprite GetChar(char c);
	int GetSpaceWidth();
protected:
	unordered_map<char, Sprite> dictionary;
	int SPACE_WIDTH = 24;
};

