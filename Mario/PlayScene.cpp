#include "PlayScene.h"
#include "MarioPowerUp.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "SceneManager.h"
#include "RaccoonMario.h"
#include "GhostTile.h"
#include "CloudTile.h"
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

	this->camera = shared_ptr<Camera>(new Camera(Vec2(0, 735), CGame::GetInstance()->GetGraphic().GetViewportSize()));
	this->camera->SetTracking(mario);

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
		obj->Update();
	}

	vector<shared_ptr<IColliable>> objs;
	objs.clear();
	objs.insert(objs.end(), objects.begin(), objects.end());

	vector<shared_ptr<IColliable>> tiles = gameMap->GetColliableTileAround(mario->GetPosition(), mario->GetHitBox(), mario->GetDistance());
	objs.insert(objs.end(), tiles.begin(), tiles.end());
	objs.insert(objs.end(), mapObjects.begin(), mapObjects.end());
	mario->CollisionUpdate(&objs);	

	for (shared_ptr<GameObject> obj : objects) {
		objs.clear();

		objs.push_back(this->mario);
		objs.insert(objs.end(), objects.begin(), objects.end());

		vector<shared_ptr<IColliable>> tiles = gameMap->GetColliableTileAround(obj->GetPosition(), obj->GetHitBox(), obj->GetDistance());
		objs.insert(objs.end(), tiles.begin(), tiles.end());
		objs.insert(objs.end(), mapObjects.begin(), mapObjects.end());

		obj->CollisionUpdate(&objs);
	}

	RemoveDespawnedObjects();

	mario->StatusUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->StatusUpdate();
	}

	mario->FinalUpdate();
	for (shared_ptr<GameObject> obj : objects) {
		obj->FinalUpdate();
	}

	camera->Update();
}

void PlayScene::Render()
{
	CScene::Render();
	mario->Render();
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

void PlayScene::ColliableTilePreLoadEvent(const char* type, int id, shared_ptr<ColliableTile>& tile, MapProperties& props)
{
	if (strcmp(type, "GhostTile") == 0) {
		tile = make_shared<GhostTile>(id);
	}

	if (strcmp(type, "CloudTile") == 0) {
		tile = make_shared<CloudTile>(id);
	}
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
	__hook(&Events::ColliableTilePreLoadEvent, Events::GetInstance(), &PlayScene::ColliableTilePreLoadEvent);
	__hook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ColliableTilePreLoadEvent, Events::GetInstance(), &PlayScene::ColliableTilePreLoadEvent);
	__unhook(&Events::ObjectLoadEvent, Events::GetInstance(), &PlayScene::ObjectLoadEvent);
}
