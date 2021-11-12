#pragma once
#include "Utils.h"
#include "Texture.h"

class TextureManager
{
private:
	/*Define Singleton*/
	static TextureManager* __instance;
	TextureManager();

	/*Class content*/
	unordered_map<string, LPTEXTURE> textures;

public:
	virtual void Add(string id, LPCWSTR filePath);

	virtual LPTEXTURE Get(string i);

	static TextureManager* GetInstance();
	~TextureManager();
};

