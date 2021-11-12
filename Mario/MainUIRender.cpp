#include "MainUI.h"
#include "MarioGame.h"
#include "SceneManager.h"

void MainUI::GameStartRender()
{
}

void MainUI::GameIntroRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DXCOLOR(0, 0, 0, 255));
	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	SceneManager::GetInstance()->Render(D3DXCOLOR(255, 255, 255, 255));
	worldViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));
	fullViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));
}

void MainUI::GameMenuRender()
{
}

void MainUI::GameWorldMapRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DXCOLOR(0, 0, 0, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DXCOLOR(255, 255, 255, 255));
	worldViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DXCOLOR(255, 255, 255, 255));
	hudViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));
}

void MainUI::GamePlayRender(GameState)
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DXCOLOR(0, 0, 0, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DXCOLOR(255, 255, 255, 255));
	worldViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DXCOLOR(255, 255, 255, 255));
	hudViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));
}

void MainUI::GameOverRender()
{
	MarioGame::GetInstance()->GetGraphic().Clear(D3DXCOLOR(0, 0, 0, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(worldView);
	SceneManager::GetInstance()->Render(D3DXCOLOR(255, 255, 255, 255));
	worldViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(hud);
	hud->Render(D3DXCOLOR(255, 255, 255, 255));
	hudViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));

	MarioGame::GetInstance()->GetGraphic().SetViewport(fullView);
	fullViewEffect->Render(D3DXCOLOR(255, 255, 255, 255));
}