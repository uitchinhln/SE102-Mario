#pragma once
#include "Utils.h"
#include "StopWatch.h"

class Mario;
class Goomba;
class Koopas;
class RaccoonLeaf;
class RedMushroom;
class GreenMushroom;
class Mario;
class IntroScene;
class MarioKeyboard;

class IntroController
{
private:
	static IntroController* __instance;

	int step = 0;

	int marioStep = 0;
	Stopwatch marioTimer;

	int luigiStep = 0;
	Stopwatch luigiTimer;

	int itemStep = 0;
	Stopwatch itemTimer;

	Stopwatch timer;

	IntroScene* scene;

	shared_ptr<Goomba> goomba;
	shared_ptr<Koopas> koopas;
	shared_ptr<Koopas> koopas2;
	shared_ptr<RaccoonLeaf> leaf;
	shared_ptr<RedMushroom> mushroom;
	shared_ptr<GreenMushroom> greenmushroom;

	shared_ptr<Mario> luigi, mario;
	shared_ptr<MarioKeyboard> luigiKeyboad, marioKeyboard;

public:
	IntroController();

	void Reset();

	void Update();

	void SetStep(int value);

	void SetScene(IntroScene* scene);

	void MarioUpdate();

	void LuigiUpdate();

	void ItemUpdate();

	IntroScene* GetScene();

	static IntroController* GetInstance();
};

