#pragma once
#include "Utils.h"
#include "GameProperty.h"
#include "GameWindow.h"
#include "GameGraphic.h"
#include "KeyboardProcessor.h"
#include "GameTime.h"
#include "StopWatch.h"
#include "Font.h"

class CGame
{
public:
	virtual void LoadResources();
	virtual void Update();
	virtual void Draw();

	int Run();

	GameGraphic& GetGraphic();
	GameWindow& GetWindow();
	KeyboardProcessor& GetKeyBoard();

	static GameTime Time();

	static CGame* GetInstance();
	~CGame();

	Font* DefaultFont;

protected:
	CGame(GameProperties properties);

	unique_ptr<GameWindow> window;
	unique_ptr<GameGraphic> graphic;
	unique_ptr<KeyboardProcessor> keyboard;

	void Init(GameProperties properties);

	static CGame* __instance;

private:

	static GameTime gameTime;

	Stopwatch gameTimer = Stopwatch(TimeMode::SYSTEM_TIME);

	bool initialized = false;

	bool isRunning = false;
	
	GameProperties properties;

	bool ProcessMessage(MSG& msg);

	void Render();
};