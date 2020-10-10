#pragma once
#include "libs.h"
#include "Sprite.h"

class SpriteManager
{
	SpriteManager();
	static SpriteManager* __instance;

	unordered_map<string, Sprite> sprites;
public:
	virtual void Add(string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex);

	virtual Sprite Get(string id);

	static SpriteManager* GetInstance();
	~SpriteManager();
};

