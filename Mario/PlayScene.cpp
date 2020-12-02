#include "PlayScene.h"
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

void PlayScene::LoadFromXml(TiXmlElement* data)
{
	string mapPath = data->FirstChildElement("TmxMap")->Attribute("path");
	string mapName = data->FirstChildElement("TmxMap")->Attribute("fileName");

	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
	this->mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));

	this->camera = make_shared<Camera>(camPos, camSize);
	this->camera->SetTracking(mario);

	this->hud = make_shared<Hud>(hudPos, hudSize);

	this->gameMap = CGameMap::FromTMX(mapPath, mapName);
	this->gameMap->SetCamera(camera);

	mario->SetPowerUp(make_shared<Small>(mario));
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
	for (shared_ptr<GameObject> obj : objects) {
		//if (!obj->GetCollisionCalc()->AABB(obj->GetHitBox(), camera->GetBoundingBox())) continue;
		obj->Update();
	}

	vector<shared_ptr<IColliable>> objs;
	objs.clear();
	objs.insert(objs.end(), objects.begin(), objects.end());
	objs.insert(objs.end(), mapObjects.begin(), mapObjects.end());
	mario->CollisionUpdate(&objs);	

	objs.push_back(this->mario);
	for (shared_ptr<GameObject> obj : objects) {
		//if (!obj->GetCollisionCalc()->AABB(obj->GetHitBox(), camera->GetBoundingBox())) continue;
		obj->CollisionUpdate(&objs);
	}

	mario->StatusUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		//if (!obj->GetCollisionCalc()->AABB(obj->GetHitBox(), camera->GetBoundingBox())) continue;
		obj->StatusUpdate();
	}

	mario->FinalUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		//if (!obj->GetCollisionCalc()->AABB(obj->GetHitBox(), camera->GetBoundingBox())) continue;
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

	CScene::Render();
	mario->Render();

	CGame::GetInstance()->GetGraphic().SetViewport(hud);

	hud->Render();
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"PlayScene Render: %d\t%d\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
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
	//DebugOut(L"Object: %s\n", ToLPCWSTR(type));
	//GameObjects
	if (strcmp(type, MEntityType::Goomba.ToString().c_str()) == 0) {
		SpawnEntity(Goomba::CreateGoomba(fixedPos));
	}
	if (strcmp(type, MEntityType::Koopas.ToString().c_str()) == 0) {
		SpawnEntity(Koopas::CreateKoopas(fixedPos));
	}
	if (strcmp(type, MEntityType::EndmapReward.ToString().c_str()) == 0) {
		SpawnEntity(EndmapReward::CreateEndmapReward(fixedPos));
	}
	if (strcmp(type, MEntityType::QuestionBlock.ToString().c_str()) == 0) {
		SpawnEntity(QuestionBlock::CreateQuestionBlock(fixedPos));
	}

	//MapObjects
	if (strcmp(type, MEntityType::SolidBlock.ToString().c_str()) == 0) {
		mapObjects.push_back(SolidBlock::CreateSolidBlock(fixedPos, size));
	}
	if (strcmp(type, MEntityType::GhostBlock.ToString().c_str()) == 0) {
		mapObjects.push_back(GhostBlock::CreateGhostBlock(fixedPos, size));
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
