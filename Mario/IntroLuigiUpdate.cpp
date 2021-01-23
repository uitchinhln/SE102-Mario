#include "IntroController.h"
#include "Mario.h"
#include <dinput.h>
#include "Koopas.h"
#include "CrouchKoopas.h"
#include "IntroScene.h"
#include "MovingShell.h"

void IntroController::LuigiUpdate()
{
	if (luigiStep == 0) {
		luigiKeyboad->PressKey(DIK_RIGHT);
		luigiStep = 1;
	}
	else if (luigiStep == 1) {
		if (luigi->GetPosition().x > 144) {

			luigi->OnKeyDown(DIK_S);
			luigiKeyboad->PressKey(DIK_S);
			luigiKeyboad->ReleaseKey(DIK_RIGHT);

			luigiStep = 2;
		}
	} 
	else if (luigiStep == 2) {
		if (luigiTimer.Elapsed() > 3200) {
			luigiKeyboad->ReleaseKey(DIK_S);
			luigiStep = 3;
		}
	} 
	else if (luigiStep == 3) {
		if (luigi->IsOnGround()) {
			luigi->OnKeyDown(DIK_S);
			luigiKeyboad->PressKey(DIK_S);
			luigiKeyboad->PressKey(DIK_RIGHT);
			luigiStep = 4;
		}
	} 
	else if (luigiStep == 4) {
		if (luigi->GetPosition().x > 768) {
			luigi->SetCollidibility(false);
			luigiKeyboad->ReleaseKey(DIK_S);
			luigiKeyboad->ReleaseKey(DIK_RIGHT);
			luigiStep = 5;
		}
	} 
	else if (luigiStep == 5) {
		if (luigiTimer.Elapsed() > 11500) {
			luigiKeyboad->PressKey(DIK_LEFT);
			luigiStep = 6;
		}
	} 
	else if (luigiStep == 6) {
		if (luigi->GetPosition().x < 768) {
			luigi->SetCollidibility(true);
			koopas2 = Koopas::CreateKoopas(luigi->GetPosition());
			koopas2->SetPower(make_shared<CrouchKoopas>(koopas2));
			koopas2->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(koopas2);

			luigiKeyboad->PressKey(DIK_A);
			luigiStep = 7;
		}
	} 
	else if (luigiStep == 7) {
		luigi->SetCollidibility(false);
		if (luigi->GetPosition().x < 648) {
			luigiKeyboad->ReleaseKey(DIK_LEFT);
			luigiStep = 8;
		}
	} 
	else if (luigiStep == 8) {
		if (luigiTimer.Elapsed() > 12884) {
			luigi->SetCollidibility(true);
			luigiKeyboad->ReleaseKey(DIK_A);
			luigiStep = 9;
		}
	} 
	else if (luigiStep == 9) {
		if (luigi->GetPosition().x - koopas2->GetPosition().x < 144 && koopas2->GetVelocity().x > 0) {
			luigiKeyboad->PressKey(DIK_RIGHT);
			luigi->GetVelocity().x = 0.5f;
			luigi->SetCollidibility(false);
			luigiStep = 10;
		}
	} 
}
