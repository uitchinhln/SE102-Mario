#pragma once
#include "Utils.h"
#include "Sprite.h"

class SpriteManager
{
	SpriteManager();
	static SpriteManager* __instance;

	unordered_map<string, Sprite> sprites;
public:
	virtual Sprite Add(string id, int left, int top, int width, int height, int xpivot, int ypivot, LPDIRECT3DTEXTURE9 tex);

	virtual Sprite Get(string id);

	virtual void ImportFromXml(const char* filePath);

	static SpriteManager* GetInstance();
	~SpriteManager();
};

