#pragma once
#include "Utils.h"

class TextureManager
{
private:
	/*Define Singleton*/
	static TextureManager* __instance;
	TextureManager();

	/*Class content*/
	unordered_map<string, LPDIRECT3DTEXTURE9> textures;

public:
	virtual void Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	virtual LPDIRECT3DTEXTURE9 Get(string i);

	static LPDIRECT3DTEXTURE9 Load(LPCWSTR texturePath, D3DCOLOR transparentColor);
	static TextureManager* GetInstance();
	~TextureManager();
};

