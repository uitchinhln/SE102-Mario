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
	void Reset();

	void Update();

	void SetStep(int value);

	void SetScene(IntroScene* scene);

	IntroScene* GetScene();

	static IntroController* GetInstance();

	Mario* mario1, *mario2;
};

