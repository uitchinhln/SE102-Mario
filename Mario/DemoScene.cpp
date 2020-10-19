#include "DemoScene.h"
#include "Game.h"
#include "Mario.h"

void DemoScene::Load()
{
	this->camera = shared_ptr<Camera>(new Camera(Vec2(0, 735), CGame::GetInstance()->GetWindow().GetWindowSize()));
	this->camera->HookEvent();

	this->gameMap = CGameMap::FromTMX("Resource/Xml/NewMap/BigMap", "world-1-1-map.tmx");
	this->gameMap->SetCamera(camera);

	this->objects.push_back(make_shared<Mario>());
}

void DemoScene::Update()
{
	camera->Update();
	CScene::Update();
}
