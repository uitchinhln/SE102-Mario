#pragma once
#include "Utils.h"
#include "Transform.h"
#include "Texture.h"

class Viewport;

class GameGraphic
{
	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	LPD3DX10SPRITE spriteObject;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????

public:
	void Init(HWND hwnd);

	ID3D10Device* GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView; }
	ID3D10BlendState* GetAlphaBlending() { return this->pBlendStateAlpha; }
	LPD3DX10SPRITE GetSpriteHandler() { return this->spriteObject; }

	void Clear(D3DXCOLOR color);

	void Draw(float x, float y, D3DXVECTOR3 pivot, LPTEXTURE texture, RECT r, Transform& transform, D3DXCOLOR overlay = D3DXCOLOR(255, 255, 255, 255));

	//void DrawText(LPD3DXFONT font, );

	LPTEXTURE CreateTextureFromFile(LPCWSTR texturePath);

	void SetViewport(shared_ptr<Viewport> viewport);

	void ClipScene();

	~GameGraphic();
};

