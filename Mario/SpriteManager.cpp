#include "SpriteManager.h"

SpriteManager* SpriteManager::__instance = nullptr;

void SpriteManager::Add(string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	sprites[id] = new CSprite(id, left, top, width, height, tex);
}

Sprite SpriteManager::Get(string id)
{
	return sprites[id];
}

SpriteManager* SpriteManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new SpriteManager();
	}

	return __instance;
}

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}
