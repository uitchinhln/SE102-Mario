#include "MainUI.h"
#include "MarioGame.h"
#include "SceneManager.h"

void MainUI::GameStartRender()
{
}

void MainUI::GameIntroRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));
	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	SceneManager::GetInstance()->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	worldViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	fullViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
}

void MainUI::GameMenuRender()
{
}

void MainUI::GameWorldMapRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	worldViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	hudViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
}

void MainUI::GamePlayRender(GameState)
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	worldViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	hudViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
}

void MainUI::GameOverRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DCOLOR_XRGB(0, 0, 0));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	worldViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
	hudViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DCOLOR_ARGB(255, 255, 255, 255));
}