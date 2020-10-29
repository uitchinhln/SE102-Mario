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
	mario->Update(&gameMap->GetColliableTileAround(mario->GetPosition(), mario->GetHitBox(), mario->GetDistance()));
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

void PlayScene::ColliableTilePreLoadEvent(const char* type, int id, shared_ptr<ColliableTile>& tile)
{
	if (strcmp(type, "GhostTile") == 0) {
		tile = make_shared<GhostTile>(id);
	}

	if (strcmp(type, "CloudTile") == 0) {
		tile = make_shared<CloudTile>(id);
	}
}

void PlayScene::HookEvent()
{
	__hook(&Events::ColliableTilePreLoadEvent, Events::GetInstance(), &PlayScene::ColliableTilePreLoadEvent);
}

void PlayScene::UnhookEvent()
{
	__unhook(&Events::ColliableTilePreLoadEvent, Events::GetInstance(), &PlayScene::ColliableTilePreLoadEvent);
}
