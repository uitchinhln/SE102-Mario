#include "IntroController.h"
#include "MarioGame.h"
#include "Mario.h"
#include "MarioKeyboard.h"
#include "MainUI.h"
#include "IntroScene.h"
#include "EffectServer.h"
#include "CurtainFX.h"
#include "ThreeFX.h"
#include "Mario.h"
#include "TreeRight.h"
#include "TreeLeft.h"
#include "MenuDialog.h"
#include "Small.h"
#include "BigMario.h"
#include "Goomba.h"

IntroController* IntroController::__instance = nullptr;

IntroController::IntroController()
{
	this->mario = make_shared<Mario>();
	this->mario->SetKeyboard(marioKeyboard = make_shared<MarioKeyboard>());
	this->mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	this->mario->SetPowerUp(make_shared<BigMario>(mario));
	this->mario->MovingBound = RectF(-100, -100, 1000, 1000);
	this->marioKeyboard->SetGlobalKeyBoardEnable(false);

	this->luigi = make_shared<Mario>();
	this->luigi->SetKeyboard(luigiKeyboad = make_shared<MarioKeyboard>());
	this->luigi->SetCollisionCalculator(make_shared<CollisionCalculator>(luigi));
	this->luigi->SetPowerUp(make_shared<BigMario>(luigi));
	this->luigi->MovingBound = RectF(-100, -100, 1000, 1000);
	this->luigiKeyboad->SetGlobalKeyBoardEnable(false);
}

void IntroController::Reset()
{
	step = 0;
	marioStep = 0;
	luigiStep = 0;
	itemStep = 0;
	marioTimer.Restart();
	luigiTimer.Restart();
	itemTimer.Restart();

	goomba.reset();
	koopas.reset();
	koopas2.reset();
	leaf.reset();
	mushroom.reset();
	greenmushroom.reset();
}

void IntroController::Update()
{
	if (step == 0) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<CurtainFX>(2000, [](long playTime) {
			if (playTime == 0) {
				IntroController::GetInstance()->SetStep(2);
			}
			if (playTime == 1) {
				IntroController::GetInstance()->SetStep(4);
			}
			}));
		step = 1;
	}
	else if (step == 1) {
	}
	else if (step == 2) {
		luigi->SetPosition(Vec2(0, 479));
		luigi->SetFacing(1);
		scene->SpawnEntityWithoutGrid(luigi);

		mario->SetPosition(Vec2(721, 479));
		mario->SetFacing(-1);
		scene->SpawnEntityWithoutGrid(mario);

		timer.Restart();
		step = 3;
	}
	else if (step == 3) {
	}
	else if (step == 4) {
		MarioUpdate();
		LuigiUpdate();
		ItemUpdate();
	}
}

void IntroController::SetStep(int value)
{
	this->step = value;
}

void IntroController::SetScene(IntroScene* scene)
{
	this->scene = scene;
}

IntroScene* IntroController::GetScene()
{
	return scene;
}

IntroController* IntroController::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new IntroController();
	}
	return __instance;
}
