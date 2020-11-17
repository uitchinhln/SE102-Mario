#pragma once
#include "Utils.h"
#include "Transform.h"

class Viewport;

class GameGraphic
{
	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;

public:
	void Init(D3DPRESENT_PARAMETERS d3dpp, HWND hwnd);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	Vec2 GetSceneSize();

	void Clear(D3DCOLOR color);

	void Draw(float x, float y, D3DXVECTOR3 pivot, LPDIRECT3DTEXTURE9 texture, RECT r, Transform& transform, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPCWSTR texturePath, D3DCOLOR transparentColor);

	void SetViewport(shared_ptr<Viewport> viewport);

	void ClipScene();

	~GameGraphic();
};

