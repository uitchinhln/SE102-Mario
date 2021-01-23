#include "IntroController.h"
#include "ThreeFX.h"
#include "IntroScene.h"
#include "EffectServer.h"
#include "Goomba.h"
#include "Koopas.h"
#include "GreenMushroom.h"
#include "RaccoonLeaf.h"
#include "TreeLeft.h"
#include "TreeRight.h"
#include "CrouchKoopas.h"
#include "RedCrouchKoopas.h"

void IntroController::ItemUpdate()
{
	if (itemStep == 0) {
		if (itemTimer.Elapsed() > 4300) {
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
					//MarioGame::GetInstance()->GetMainUI()->ActiveDialog(make_shared<MenuDialog>());
				}					
				});
			IntroController::GetInstance()->GetScene()->GetBackgroundEffect()->SpawnEffect(fx);
			itemStep = 1;
		}
	}
	else if (itemStep == 1) {
		if (itemTimer.Elapsed() > 5500) {
			goomba = Goomba::CreateGoomba(Vec2(192, 0));
			goomba->SetVelocity(Vec2(0, 0));
			goomba->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(goomba);

			mushroom = RedMushroom::CreateRedMushroom(Vec2(96, 0));
			mushroom->SetMovingStep(2);
			mushroom->SetVelocity(Vec2(0, 0.5));
			mushroom->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(mushroom);

			greenmushroom = GreenMushroom::CreateGreenMushroom(Vec2(624, 0));
			greenmushroom->SetMovingStep(2);
			greenmushroom->SetVelocity(Vec2(0, 0.5));
			greenmushroom->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(greenmushroom);

			leaf = RaccoonLeaf::CreateRaccoonLeaf(Vec2(336, 35));
			leaf->Visible = true;
			leaf->SetMovingStep(1);
			leaf->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(leaf);

			koopas = Koopas::CreateKoopas(Vec2(mario->GetPosition().x, 30));
			koopas->SetPower(make_shared<CrouchKoopas>(koopas));
			koopas->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(koopas);

			koopas2 = Koopas::CreateKoopas(Vec2(528, 30));
			koopas2->SetPower(make_shared<RedCrouchKoopas>(koopas2, true));
			koopas2->SetRenderOrder(1002);
			scene->SpawnEntityWithoutGrid(koopas2);

			itemStep = 2;
		}
	}
	else if (itemStep == 2) {
		if (mushroom->GetVelocity().y == 0) {
			mushroom->SetVelocity(Vec2(-0.15, 0));
		}
		if (greenmushroom->GetVelocity().y == 0) {
			greenmushroom->SetVelocity(Vec2(0.15, 0));
		}
		if (mario->GetObjectType() == MEntityType::RaccoonMario) {
			goomba->SetVelocity(Vec2(-0.04, 0.5));
		}
		if (itemTimer.Elapsed() < 10000 && koopas->GetPosition().x - 60 < goomba->GetPosition().x && koopas->GetObjectType() == MEntityType::KoopasImposter) {
			koopas->SetPower(make_shared<CrouchKoopas>(koopas));
			koopas->SetRenderOrder(1702);
		}
	}
}