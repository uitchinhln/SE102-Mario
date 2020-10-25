#include "PlayScene.h"
#include "MarioPowerUp.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "SceneManager.h"
#include "StateManager.h"
#include "RaccoonMario.h"

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

	TiXmlElement* marios = data->FirstChildElement("Marios");
	for (TiXmlElement* node = marios->FirstChildElement(); node != NULL; node = node->NextSiblingElement()) {
		shared_ptr<MarioPowerUp> marioPower;

		if (strcmp(node->Value(), "SmallMario") == 0)
			marioPower = make_shared<Small>(mario);
		if (strcmp(node->Value(), "BigMario") == 0)
			marioPower = make_shared<BigMario>(mario);
		if (strcmp(node->Value(), "FireMario") == 0)
			marioPower = make_shared<FireMario>(mario);
		if (strcmp(node->Value(), "RaccoonMario") == 0)
			marioPower = make_shared<RaccoonMario>(mario);
		
		marioPower->Init(node);

		StateManager::GetInstance()->Add(node->Value(), marioPower);
	}

	mario->SetPowerUp(StateManager::GetInstance()->Get("SmallMario"));
}

void PlayScene::Load()
{
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
