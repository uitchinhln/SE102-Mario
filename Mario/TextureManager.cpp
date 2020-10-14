#include "TextureManager.h"
#include "Game.h"

TextureManager* TextureManager::__instance = nullptr;

LPDIRECT3DTEXTURE9 TextureManager::Load(LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	HRESULT result = D3DXGetImageInfoFromFile(texturePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", texturePath);
		return NULL;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,			// Transparent color
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed. File: %s\n", texturePath);
		return NULL;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
	return texture;
}

void TextureManager::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	this->textures[id] = this->Load(filePath, transparentColor);
}

LPDIRECT3DTEXTURE9 TextureManager::Get(string i)
{
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
	if (__instance) {
		delete __instance;
	}
	__instance = nullptr;
}
