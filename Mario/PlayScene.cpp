#include "PlayScene.h"
#include "Game.h"
#include "MarioPowerUp.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "SceneManager.h"
#include "RaccoonMario.h"
#include "Events.h"

#include "Goomba.h"
#include "Koopas.h"

#include "EndmapReward.h"
#include "QuestionBlock.h"

#include "SolidBlock.h"
#include "GhostBlock.h"
#include "Pipe.h"
#include "Spawner.h"
#include "JumpingKoopas.h"
#include "DefRedKoopas.h"
#include "Venus.h"
#include "RedVenus.h"
#include "Piranha.h"
#include "RedGoomba.h"
#include "Coin.h"
#include "VoidBlock.h"

void PlayScene::LoadFromXml(TiXmlElement* data)
{
	string mapPath = data->FirstChildElement("TmxMap")->Attribute("path");
	string mapName = data->FirstChildElement("TmxMap")->Attribute("fileName");

	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
	this->mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	this->mario->SetPowerUp(make_shared<Small>(mario));

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	this->hud = make_shared<Hud>(hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath, mapName);
	this->gameMap->SetCamera(camera);
}

void PlayScene::Load()
{
	HookEvent();
}

void PlayScene::Unload()
{
	CScene::Unload();
	UnhookEvent();
}

void PlayScene::Update()
{
	gameMap->Update();

	mario->Update();

	RectF cam = camera->GetBoundingBox();

	for (shared_ptr<GameObject> obj : objects) {
		obj->Update();
	}

	vector<shared_ptr<GameObject>> objs;
	objs.clear();
	objs.insert(objs.end(), objects.begin(), objects.end());
	objs.insert(objs.end(), mapObjects.begin(), mapObjects.end());
	mario->CollisionUpdate(&objs);	

	objs.push_back(this->mario);
	for (shared_ptr<GameObject> obj : objects) {
		obj->CollisionUpdate(&objs);
	}

	mario->CollisionDoubleFilter();
	for (shared_ptr<GameObject> obj : objects) {
		obj->CollisionDoubleFilter();
	}

	mario->PositionUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->PositionUpdate();
	}

	mario->RestoreCollision();
	for (shared_ptr<GameObject> obj : objects) {
		obj->RestoreCollision();
	}

	mario->PositionLateUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->PositionLateUpdate();
	}

	mario->StatusUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->StatusUpdate();
	}

	mario->FinalUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->FinalUpdate();
	}

	RemoveDespawnedObjects();

	camera->Update();
	hud->Update();
}

void PlayScene::Render()
{
	//auto start = std::chrono::high_resolution_clock::now();
	CGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));	
	CGame::GetInstance()->GetGraphic().SetViewport(camera);

	gameMap->Render();
	vector<shared_ptr<GameObject>> renderObjects;
	renderObjects.insert(renderObjects.end(), objects.begin(), objects.end());
	renderObjects.push_back(mario);

	sort(renderObjects.begin(), renderObjects.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
	});

	RectF cam = camera->GetBoundingBox();
	for each (shared_ptr<GameObject> obj in renderObjects)
	{
		Vec2 Position = obj->GetPosition();
		if (Position.x < cam.left - 200 || Position.y < cam.top - 200 || Position.x > cam.right + 200 || Position.y > cam.bottom + 200) {
			continue;
		}
		if (obj->Visible) obj->Render();
	}

	CGame::GetInstance()->GetGraphic().SetViewport(hud);

	hud->Render();
}

void PlayScene::OnKeyDown(int key)
{
	if (!mario) return;
	mario->OnKeyDown(key);
}

void PlayScene::OnKeyUp(int key)
{
	if (!mario) return;
	mario->OnKeyUp(key);
}

void PlayScene::ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props)
{
	//GameObjects
	if (strcmp(type, MEntityType::Goomba.ToString().c_str()) == 0) {
		SpawnEntity(Goomba::CreateGoomba(fixedPos));
	}
	if (strcmp(type, MEntityType::RedGoomba.ToString().c_str()) == 0) {
		SpawnEntity(RedGoomba::CreateRedGoomba(fixedPos));
	}
	if (strcmp(type, MEntityType::Koopas.ToString().c_str()) == 0) {
		SpawnEntity(Koopas::CreateKoopas(fixedPos));
	}
	if (strcmp(type, MEntityType::KoopasJumping.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<JumpingKoopas>(kp));
		SpawnEntity(kp);
	}
	if (strcmp(type, MEntityType::RedKoopas.ToString().c_str()) == 0) {
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(fixedPos);
		kp->SetPower(make_shared<DefRedKoopas>(kp));
		SpawnEntity(kp);
	}
	if (strcmp(type, MEntityType::Venus.ToString().c_str()) == 0) {
		shared_ptr<Venus> vn = Venus::CreateVenus(fixedPos);
		SpawnEntity(vn);
	}
	if (strcmp(type, MEntityType::RedVenus.ToString().c_str()) == 0) {
		shared_ptr<RedVenus> vn = RedVenus::CreateRedVenus(fixedPos);
		SpawnEntity(vn);
	}
	if (strcmp(type, MEntityType::Piranha.ToString().c_str()) == 0) {
		shared_ptr<Piranha> pi = Piranha::CreatePiranha(fixedPos);
		SpawnEntity(pi);
	}
	if (strcmp(type, MEntityType::EndmapReward.ToString().c_str()) == 0) {
		SpawnEntity(EndmapReward::CreateEndmapReward(fixedPos));
	}
	if (strcmp(type, MEntityType::QuestionBlock.ToString().c_str()) == 0) {
		SpawnEntity(QuestionBlock::CreateQuestionBlock(fixedPos, props));
	}
	if (strcmp(type, MEntityType::Spawner.ToString().c_str()) == 0) {
		SpawnEntity(Spawner::CreateSpawner(fixedPos, props));
	}
	if (strcmp(type, MEntityType::Pipe.ToString().c_str()) == 0) {
		SpawnEntity(Pipe::CreatePipe(fixedPos, size, props));
	}
	if (strcmp(type, MEntityType::Coin.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::COIN));
	}
	if (strcmp(type, MEntityType::Brick.ToString().c_str()) == 0) {
		SpawnEntity(Coin::CreateCoin(fixedPos, CoinState::BRICK));
	}

	//MapObjects
	if (strcmp(type, MEntityType::SolidBlock.ToString().c_str()) == 0) {
		mapObjects.push_back(SolidBlock::CreateSolidBlock(fixedPos, size));
	}
	if (strcmp(type, MEntityType::GhostBlock.ToString().c_str()) == 0) {
		mapObjects.push_back(GhostBlock::CreateGhostBlock(fixedPos, size));
	}
	if (strcmp(type, MEntityType::VoidBlock.ToString().c_str()) == 0) {
		mapObjects.push_back(VoidBlock::CreateVoidBlock(fixedPos, size));
	}
}

void PlayScene::HookEvent()
{
	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
}
