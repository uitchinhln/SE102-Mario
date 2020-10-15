#include "Utils.h"
#include "Game.h"
#include "GameMap.h"
#include "AnimationManager.h"
#include "TextureManager.h"

#define WINDOW_CLASS_NAME L"Mario_Name"
#define MAIN_WINDOW_TITLE L"Mario_Title"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 0, 0)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAME_RATE 120


/*
	Load all game resources.
*/
void LoadResources()
{
	TextureManager::GetInstance()->Add("tex-mario", L"Resource/Xml/Textures/Mario/Mario_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Mario/MarioDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Mario/MarioAnim.xml");
	TextureManager::GetInstance()->Add("tex-enemy", L"Resource/Xml/Textures/Enemy/enemy_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	TextureManager::GetInstance()->Add("tex-intro", L"Resource/Xml/Textures/Misc/Intro/intro_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	TextureManager::GetInstance()->Add("tex-misc", L"Resource/Xml/Textures/Misc/Misc (Goods_Item)/misc_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	TextureManager::GetInstance()->Add("tex-ui", L"Resource/Xml/Textures/Misc/UI/UI_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	CGame::GetInstance()->gameMap = CGameMap::FromTMX("Resource/Xml/NewMap/BigMap", "world-1-1-map.tmx");
	CGame::GetInstance()->gameMap->SetCamera(new Camera(Vec2(0, 735) ,Vec2(SCREEN_WIDTH, SCREEN_HEIGHT)));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGame::GetInstance()->Init(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE);
	LoadResources();
	CGame::GetInstance()->Run(MAX_FRAME_RATE);

	return 0;
}