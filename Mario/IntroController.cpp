#include "IntroController.h"
#include "IntroScene.h"

IntroController* IntroController::__instance = nullptr;

void IntroController::Update()
{
	
}

void IntroController::SetStep(int value)
{
	this->step = value;
}

void IntroController::SetScene(IntroScene* scene)
{
	this->scene = scene;
}

IntroController* IntroController::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new IntroController();
	}
	return __instance;
}
