#include "Game.h"
#include "AnimationManager.h"

CGame * CGame::__instance = NULL;

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, RECT r, Vec2 scale, float rotation, D3DCOLOR overlay)
{
	D3DXVECTOR3 p(x, y, 0);
	spriteHandler->Draw(texture, &r, NULL, &p, overlay);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void CGame::Update(DWORD dt)
{
	Vec2 old = this->gameMap->GetCamera()->GetPosition();
	this->gameMap->GetCamera()->Update(dt);
}

/*
	Render a frame
*/
void CGame::Render()
{
	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(backBuffer, NULL, backgroundColor);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		gameMap->Render();

		string id = "ani-raccoon-mario-fly";
		AnimationManager::GetInstance()->Get(id)->SetPlayScale(2.0f);
		AnimationManager::GetInstance()->Get(id)->GetTransform()->Position.x = 100;
		AnimationManager::GetInstance()->Get(id)->GetTransform()->Position.y = 100;
		AnimationManager::GetInstance()->Get(id)->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}
	       
	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

int CGame::Run(int frameRate)
{
	if (isRunning) return 0;
	isRunning = true;

	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / frameRate;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			frameStart = GetTickCount() - tickPerFrame;
		}

		DWORD now = GetTickCount();
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			gameTime += now - frameStart;
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	isRunning = false;
	return 1;
}

void CGame::SetBackgroundColor(int r, int g, int b, int a)
{
	this->backgroundColor = D3DCOLOR_ARGB(a, r, g, b);
}

void CGame::SetBackgroundColor(D3DCOLOR color)
{
	this->backgroundColor = color;
}

DWORD CGame::CurrentGameTime()
{
	return this->gameTime;
}


CGame::CGame() {

}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}


CGame *CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}


LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND CGame::CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight, LPCWSTR WinClassName, LPCWSTR Title)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WinClassName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WinClassName,
			Title,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			(GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight, LPCWSTR WinClassName, LPCWSTR Title)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = this->CreateGameWindow(hInstance, nCmdShow, ScreenWidth, ScreenHeight, WinClassName, Title);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}
