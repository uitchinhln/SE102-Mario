#include "TextureManager.h"
#include "Game.h"

TextureManager* TextureManager::__instance = nullptr;

void TextureManager::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	this->textures[id] = CGame::GetInstance()->GetGraphic().CreateTextureFromFile(filePath, transparentColor);
}

LPDIRECT3DTEXTURE9 TextureManager::Get(string i)
{
	if (textures.find(i) == textures.end()) return nullptr;
	return textures[i];
}

TextureManager* TextureManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new TextureManager();
	}
	return __instance;
}

TextureManager::TextureManager() {

}

TextureManager::~TextureManager()
{
	for (const auto& [key, value] : textures) {
		value->Release();
	}
	__instance = nullptr;
}
