#include "IntroController.h"
#include "MarioGame.h"
#include "MainUI.h"
#include "IntroScene.h"
#include "EffectServer.h"
#include "CurtainFX.h"
#include "ThreeFX.h"
#include "Mario.h"
#include "TreeRight.h"
#include "TreeLeft.h"
#include "MenuDialog.h"

IntroController* IntroController::__instance = nullptr;

void IntroController::Reset()
{
	step = 0;
}

void IntroController::Update()
{
	if (step == 0) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<CurtainFX>(1000, [](long playTime) {
			if (playTime == 1) {
				shared_ptr<ThreeFX> fx = make_shared<ThreeFX>(Vec2(0, 0), 100000, [](long value) {
					if (value == 1) {
						IntroController::GetInstance()->GetScene()->SetBackgroundColor(D3DCOLOR_XRGB(91, 33, 0));						
					}
					if (value == 2) {
						IntroController::GetInstance()->GetScene()->SetBackgroundColor(D3DCOLOR_XRGB(160, 81, 0));
					}
					if (value == 3) {
						IntroController::GetInstance()->GetScene()->SetBackgroundColor(D3DCOLOR_XRGB(242, 158, 27));
					}
					if (value == 4) {
						IntroController::GetInstance()->GetScene()->SetBackgroundColor(D3DCOLOR_XRGB(250, 217, 163));
					}					
					if (value == 5) {
						IntroController::GetInstance()->GetScene()->SpawnEntityWithoutGrid(TreeRight::CreateTreeRight(Vec2(580, 271)));
						IntroController::GetInstance()->GetScene()->SpawnEntityWithoutGrid(TreeLeft::CreateTreeLeft(Vec2(-1, 367)));
						MarioGame::GetInstance()->GetMainUI()->ActiveDialog(make_shared<MenuDialog>());
					}					
				});
				IntroController::GetInstance()->GetScene()->GetBackgroundEffect()->SpawnEffect(fx);
			}
			}));
		step = 1;
	}
	else if (step == 1) {

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
