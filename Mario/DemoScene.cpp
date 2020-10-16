#include "DemoScene.h"
#include "Game.h"

void DemoScene::Load()
{
	this->camera = new Camera(Vec2(0, 735), CGame::GetInstance()->GetScreenSize());
	this->gameMap = CGameMap::FromTMX("Resource/Xml/NewMap/BigMap", "world-1-1-map.tmx");
	this->gameMap->SetCamera(camera);
}

void DemoScene::Update(int dt)
{
	camera->Update(dt);
	CScene::Update(dt);
}
