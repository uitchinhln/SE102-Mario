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

void CGame::OnKeyUp(int key)
{
}

void CGame::OnKeyDown(int key)
{
}

void CGame::Render()
{
	//auto start = std::chrono::high_resolution_clock::now();
	graphic->GetDirect3DDevice()->BeginScene();
	//if (SUCCEEDED(graphic->GetDirect3DDevice()->BeginScene()))
	//{
	// Clear back buffer with a color
	graphic->GetSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);

	Draw();

	graphic->GetSpriteHandler()->End();
	graphic->GetDirect3DDevice()->EndScene();
	//}	       
	// Display back buffer content to the screen
	graphic->GetDirect3DDevice()->Present(NULL, NULL, NULL, NULL);
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Loop: %d\t%d\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

int CGame::Run()
{
	if (isRunning) return 0;
	isRunning = true;

	MSG msg;
	gameTimer.Start();
	long tickPerFrame = 1000 / properties->TickRate;

	while (ProcessMessage(msg))
	{
		//tick
		long now = gameTimer.Elapsed();
		long accumulatedTime = now - gameTime.GetPreviousTicks();

		if (accumulatedTime >= tickPerFrame)
		{
			long tempTime = accumulatedTime;

			while (tempTime > 0) {
				long delta = max(0, min(tempTime, tickPerFrame));
				tempTime -= tickPerFrame;

				gameTime.ElapsedGameTime = delta * this->timescale;
				gameTime.TotalGameTime += delta * this->timescale;
				gameTime.SetPreviousTicks(now);

				keyboard->ProcessKeyboard();
				Update();
			}

			Render();
			Sleep(0);
		}
		else {
			Sleep(max(tickPerFrame - accumulatedTime, 0));
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

float CGame::GetTimeScale()
{
	return timescale;
}

void CGame::SetTimeScale(float value)
{
	this->timescale = value;
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
