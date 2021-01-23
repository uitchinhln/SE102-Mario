#include "IntroController.h"
#include <dinput.h>
#include "Mario.h"
#include "Koopas.h"


void IntroController::MarioUpdate()
{
	if (marioStep == 0) {
		marioKeyboard->PressKey(DIK_LEFT);
		marioStep = 1;
	}
	else if (marioStep == 1) {
		if (mario->GetPosition().x < 382) {

			marioKeyboard->ReleaseKey(DIK_LEFT);
			marioKeyboard->PressKey(DIK_DOWN);

			marioStep = 2;
		}
	}
	else if (marioStep == 2) {
		if (marioTimer.Elapsed() > 4300) {
			mario->OnKeyUp(DIK_DOWN);
			marioKeyboard->ReleaseKey(DIK_DOWN);
			marioStep = 3;
		}
	}
	else if (marioStep == 3) {
		if (marioTimer.Elapsed() > 8200) {
			mario->OnKeyDown(DIK_S);
			marioKeyboard->PressKey(DIK_S);
			marioStep = 4;
		}
	}
	else if (marioStep == 4) {
		if (mario->GetObjectType() == MEntityType::RaccoonMario) {
			mario->GetPosition().y = 220;
			mario->OnKeyUp(DIK_S);
			marioKeyboard->ReleaseKey(DIK_S);
			marioKeyboard->PressKey(DIK_X);
			marioKeyboard->PressKey(DIK_LEFT);
			marioStep = 5;
		}
	}
	else if (marioStep == 5) {
		if (mario->IsOnGround()){
			marioKeyboard->ReleaseKey(DIK_X);
			marioKeyboard->ReleaseKey(DIK_LEFT);
			marioKeyboard->PressKey(DIK_RIGHT);
			marioStep = 6;
		}
	}
	else if (marioStep == 6) {
		if (mario->GetPosition().x > 528){
			mario->GetVelocity().x = 0;
			marioKeyboard->ReleaseKey(DIK_RIGHT);
			marioStep = 7;
		}
	}
	else if (marioStep == 7) {
		if (luigi->GetPosition().x < 750) {
			marioKeyboard->PressKey(DIK_LEFT);
			marioStep = 8;
		}
	}
	else if (marioStep == 8) {
		if (mario->GetPosition().x + 70 > koopas2->GetPosition().x && !koopas2->GetHolder().lock()) {
			marioKeyboard->PressKey(DIK_X);
			marioKeyboard->ReleaseKey(DIK_LEFT);
			marioStep = 9;
		}
	}
	else if (marioStep == 9) {
		if (mario->IsOnGround()) {
			marioKeyboard->ReleaseKey(DIK_X);
			koopas2->SetRenderOrder(1402);
			marioStep = 10;
		}
	}
	else if (marioStep == 10) {
		if (mario->IsOnGround()) {
			marioKeyboard->PressKey(DIK_A);
			marioKeyboard->PressKey(DIK_RIGHT);
			marioStep = 11;
		}
	}
	else if (marioStep == 11) {
		if (mario->GetPosition().x > 288) {
			marioKeyboard->ReleaseKey(DIK_A);
			marioStep = 12;
		}
	}
	else if (marioStep == 12) {
		if (mario->GetPosition().x > 370) {
			marioKeyboard->ReleaseKey(DIK_RIGHT);
			marioStep = 13;
		}
	}
}