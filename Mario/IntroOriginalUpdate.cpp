#include "IntroScene.h"
#include "EffectServer.h"

#include "Goomba.h"
#include "Koopas.h"

#include "EndmapReward.h"
#include "QuestionBlock.h"

#include "SolidBlock.h"
#include "GhostBlock.h"
#include "VoidBlock.h"
#include "Pipe.h"

#include "Spawner.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "RedGoomba.h"
#include "Coin.h"

#include "BeginPortal.h"
#include "EndPortal.h"
#include "MovingPlatform.h"
#include "FlyingRedKoopas.h"
#include "BoomerangBrother.h"

void IntroScene::OriginalUpdate()
{
	RectF camBox = camera->GetBoundingBox();

	camera->Update();

	objectList.clear();
	objectList.insert(objectList.end(), objectsWithoutGrid.begin(), objectsWithoutGrid.end());

	gameMap->Update();

	for (shared_ptr<GameObject> obj : objectList) {
		obj->Update();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->CollisionUpdate(&objectList);
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->CollisionDoubleFilter();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->PositionUpdate();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->RestoreCollision();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->PositionLateUpdate();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->StatusUpdate();
	}

	for (shared_ptr<GameObject> obj : objectList) {
		obj->FinalUpdate();

		RectF objBox = obj->GetHitBox();
		if (!CollisionCalculator::CommonAABB(camBox, objBox)) {
			if (obj->GetInCamera) obj->OnLostCamera();
			obj->GetInCamera = false;
		}
		else {
			if (!obj->GetInCamera) obj->OnGetInCamera();
			obj->GetInCamera = true;
		}
	}

	RemoveDespawnedObjects();
}



void IntroScene::OriginalRender(D3DCOLOR overlay)
{
	sort(objectList.begin(), objectList.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
		});

	for each (shared_ptr<GameObject> obj in objectList)
	{
		if (obj->Visible && obj->IsActive()) {
			obj->Render(overlay);
		}
	}
}


void IntroScene::ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	//GameObjects
	if (strcmp(type, MEntityType::Goomba.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Goomba::CreateGoomba(fixedPos));
	}
	if (strcmp(type, MEntityType::RedGoomba.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(RedGoomba::CreateRedGoomba(fixedPos));
	}
	if (strcmp(type, MEntityType::Koopas.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Koopas::CreateKoopas(fixedPos));
	}
	if (strcmp(type, MEntityType::KoopasJumping.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<JumpingKoopas>(kp));
		SpawnEntityWithoutGrid(kp);
	}
	if (strcmp(type, MEntityType::RedKoopas.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<DefRedKoopas>(kp));
		SpawnEntityWithoutGrid(kp);
	}
	if (strcmp(type, MEntityType::RedKoopasFlying.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<FlyingRedKoopas>(kp, fixedPos, props.GetFloat("Range", 196.0f)));
		SpawnEntityWithoutGrid(kp);
	}
	if (strcmp(type, MEntityType::Venus.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Venus::CreateVenus(fixedPos));
	}
	if (strcmp(type, MEntityType::RedVenus.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(RedVenus::CreateRedVenus(fixedPos));
	}
	if (strcmp(type, MEntityType::Piranha.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Piranha::CreatePiranha(fixedPos));
	}
	if (strcmp(type, MEntityType::BoomerangBrother.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(BoomerangBrother::CreateBoomerangBrother(fixedPos, props));
	}
	if (strcmp(type, MEntityType::EndmapReward.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(EndmapReward::CreateEndmapReward(fixedPos));
	}
	if (strcmp(type, MEntityType::QuestionBlock.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(QuestionBlock::CreateQuestionBlock(fixedPos, props));
	}
	if (strcmp(type, MEntityType::Spawner.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Spawner::CreateSpawner(fixedPos, props));
	}
	if (strcmp(type, MEntityType::Pipe.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Pipe::CreatePipe(fixedPos, size, props));
	}
	if (strcmp(type, MEntityType::Coin.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Coin::CreateCoin(fixedPos, CoinState::COIN));
	}
	if (strcmp(type, MEntityType::Brick.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(Coin::CreateCoin(fixedPos, CoinState::BRICK));
	}
	if (strcmp(type, MEntityType::MovingPlatform.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(MovingPlatform::CreateMovingPlatform(fixedPos, size, props));
	}

	//MapObjects
	if (strcmp(type, MEntityType::BeginPortal.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(BeginPortal::CreatePortal(fixedPos, size, props));
	}
	if (strcmp(type, MEntityType::EndPortal.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(EndPortal::CreatePortal(fixedPos, size, props));
	}
	if (strcmp(type, MEntityType::SolidBlock.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(SolidBlock::CreateSolidBlock(fixedPos, size));
	}
	if (strcmp(type, MEntityType::GhostBlock.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(GhostBlock::CreateGhostBlock(fixedPos, size));
	}
	if (strcmp(type, MEntityType::VoidBlock.ToString().c_str()) == 0) {
		SpawnEntityWithoutGrid(VoidBlock::CreateVoidBlock(fixedPos, size));
	}
}