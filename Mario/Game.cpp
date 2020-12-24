#include "Game.h"
#include <chrono>
#include <thread>

CGame* CGame::__instance = nullptr;
GameTime CGame::gameTime = GameTime();

void CGame::LoadResources()
{
}

void CGame::Update()
{
}

void CGame::Draw()
{
}

void CGame::Render()
{
	graphic->GetDirect3DDevice()->BeginScene();
	//if (SUCCEEDED(graphic->GetDirect3DDevice()->BeginScene()))
	//{
		// Clear back buffer with a color
		graphic->GetSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);

		//auto start = std::chrono::high_resolution_clock::now();
		Draw();
		//auto finish = std::chrono::high_resolution_clock::now();
		//DebugOut(L"Loop: %d\t%d\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());

		graphic->GetSpriteHandler()->End();
		graphic->GetDirect3DDevice()->EndScene();
	//}	       
	// Display back buffer content to the screen
	graphic->GetDirect3DDevice()->Present(NULL, NULL, NULL, NULL);
}

int CGame::Run()
{
	if (isRunning) return 0;
	isRunning = true;

	MSG msg;
	gameTimer.Start();
	DWORD tickPerFrame = 1000 / properties->TickRate;

	while (ProcessMessage(msg))
	{
		//tick
		DWORD now = gameTimer.Elapsed();
		DWORD accumulatedTime = now - gameTime.GetPreviousTicks();

		if (accumulatedTime >= tickPerFrame)
		{
			gameTime.ElapsedGameTime = accumulatedTime;
			gameTime.TotalGameTime += accumulatedTime;
			gameTime.SetPreviousTicks(now);

			keyboard->ProcessKeyboard();
			//auto start = std::chrono::high_resolution_clock::now();
			Update();
			//auto finish = std::chrono::high_resolution_clock::now();
			//DebugOut(L"Loop: %d\t%d\n", accumulatedTime, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
			Render();
			Sleep(0);
		}
		else {
			Sleep(tickPerFrame - accumulatedTime);
		}
	}

	isRunning = false;
	return 1;
}


CGame::CGame(GameProperties properties) {
	__instance = this;
	Init(properties);
}

GameGraphic& CGame::GetGraphic()
{
	return *graphic;
}

GameWindow& CGame::GetWindow()
{
	return *window;
}

KeyboardProcessor& CGame::GetKeyBoard()
{
	return *keyboard;
}

GameTime CGame::Time()
{
	return gameTime;
}

CGame* CGame::GetInstance()
{
	return __instance;
}

CGame::~CGame()
{
}

bool CGame::ProcessMessage(MSG& msg)
{
	long now = gameTimer.Elapsed();
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		int tps = 1000 / properties->TickRate;
		if (gameTimer.Elapsed() - now >= tps) {
			//DebugOut(L"Waste detected!!!\n");
			gameTime.SetPreviousTicks(gameTimer.Elapsed() - tps);
		}
	}
	return true;
}

void CGame::Init(GameProperties properties)
{
	if (initialized) return;

	window = make_unique<GameWindow>(properties);
	window->CreateGameWindow();

	graphic = make_unique<GameGraphic>();
	graphic->Init(properties->d3dpp, window->GetWindowHandler());

	keyboard = make_unique<KeyboardProcessor>();
	keyboard->InitKeyboard(window->GetWindowHandler());

	this->properties = properties;

	initialized = true;
}
