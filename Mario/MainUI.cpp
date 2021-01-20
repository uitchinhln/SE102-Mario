#include "MainUI.h"
#include "SceneManager.h"
#include "MarioGame.h"

MainUI::MainUI()
{
	worldView = make_shared<Viewport>(Vec2(0, 0), Vec2(769, 579));
	fullView = make_shared<Viewport>(Vec2(0, 0), Vec2(769, 721));
}

void MainUI::Update()
{
	SceneManager::GetInstance()->GetActiveScene()->Update();
	hud->Update();
}

void MainUI::Render()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->GetActiveScene()->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
}

void MainUI::GameBeginUpdate()
{
}

void MainUI::GameBeginRender()
{
}

void MainUI::CreateHUD(TiXmlElement* node)
{
	Vec2 pos;
	string path = node->Attribute("path");

	node->QueryFloatAttribute("left", &pos.x);
	node->QueryFloatAttribute("top", &pos.y);

	hud = make_shared<Hud>(path, pos, Vec2(769, 142));
}
