#pragma once
#include "Utils.h"

class Mario;
class IntroScene;

class IntroController
{
private:
	static IntroController* __instance;

	int step = 0;

	IntroScene* scene;

public:
	void Update();

	void SetStep(int value);

	void SetScene(IntroScene* scene);

	static IntroController* GetInstance();
};

