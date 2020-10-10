#pragma once
#include "libs.h"

class TextureManager
{
private:
	/*Define Singleton*/
	static TextureManager* __instance;
	TextureManager();

	/*Class content*/
	unordered_map<string, LPDIRECT3DTEXTURE9> textures;

protected:
	virtual LPDIRECT3DTEXTURE9 Load(LPCWSTR texturePath, D3DCOLOR transparentColor);

public:
	virtual void Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor);
	virtual LPDIRECT3DTEXTURE9 Get(string i);

	static TextureManager* GetInstance();
	~TextureManager();
};

