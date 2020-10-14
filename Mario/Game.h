#pragma once
#include "Utils.h"
#include "GameMap.h"

class CGame
{
	CGame();
	static CGame * __instance;

	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen

	HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight, LPCWSTR WinClassName, LPCWSTR Title);
	bool isRunning = false;

	DWORD gameTime = 0;

	D3DCOLOR backgroundColor = D3DCOLOR_ARGB(255, 255, 255, 255);

public:
	void Init(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight, LPCWSTR WinClassName, LPCWSTR Title);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, RECT r, Vec2 scale, float rotation, D3DCOLOR mix = D3DCOLOR_ARGB(255, 255, 255, 255));

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void Update(DWORD dt);
	void Render();
	int Run(int frameRate = 60);

	void SetBackgroundColor(int r, int g, int b, int a);
	void SetBackgroundColor(D3DCOLOR color);

	DWORD CurrentGameTime(); 

	GameMap gameMap;

	static CGame * GetInstance();

	~CGame();
};