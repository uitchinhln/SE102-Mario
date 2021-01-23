#include "Mario.h"
#include "MarioGame.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"
#include "SceneManager.h"
#include "Koopas.h"
#include "CrouchKoopas.h"
#include "Goomba.h"


void Mario::OnKeyUp(int key)
{
	//if (IsControllerLocked()) return;
	power->OnKeyUp(key);
	switch (key)
	{
	case DIK_A:
		DropShell();
		break;
	case DIK_Q:
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(Goomba::CreateGoomba(Position - Vec2(0, 230)));
		break;
	case DIK_D:
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(Position - Vec2(0, 230));
		kp->SetPower(make_shared<CrouchKoopas>(kp));
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(kp);
		break;
	}
}

void Mario::OnKeyDown(int key)
{
	//if (IsControllerLocked()) return;
	shared_ptr<MarioPower> p = power;
	p->OnKeyDown(key);

	// Change mario power
	Vec2 fixPos = Vec2(GetHitBox().left, GetHitBox().bottom);
	shared_ptr<Mario> thiss = MarioGame::GetInstance()->GetMario();

	switch (key)
	{
	case DIK_1:
		SetPowerUp(make_shared<Small>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_2:
		SetPowerUp(make_shared<BigMario>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_3:
		SetPowerUp(make_shared<FireMario>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_4:
		SetPowerUp(make_shared<RaccoonMario>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_P:
		DebugOut(L"Position: x = %f\ty = %f\n", Position.x, Position.y);
		break;
	case DIK_T:
		Position.x = 5711;
		Position.y = 1150;
		break;
	case DIK_R:
		Position.x = 6010;
		Position.y = 1030;
		SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetLimitEdge(Direction::Top, 0);
		SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position.x = 6010;
		break;
	case DIK_E:
		Position.x = 7900;
		Position.y = 1136;
		break;
	}

}