#include "Utils.h"
#include "Game.h"
#include "DemoScene.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "Events.h"

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
	TextureManager::GetInstance()->Add("tex-mario", L"Resource/Xml/Textures/Mario/Mario_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Mario/MarioDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Mario/MarioAnim.xml");
	
	TextureManager::GetInstance()->Add("tex-enemy", L"Resource/Xml/Textures/Enemy/enemy_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Enemy/EnemyDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Enemy/EnemyAnim.xml");
	
	TextureManager::GetInstance()->Add("tex-intro", L"Resource/Xml/Textures/Misc/Intro/intro_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/Intro/IntroDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/Intro/IntroAnim.xml");
	
	TextureManager::GetInstance()->Add("tex-misc", L"Resource/Xml/Textures/Misc/Misc (Goods_Item)/misc_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/Misc (Goods_Item)/MiscDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/Misc (Goods_Item)/MiscAnim.xml");
	
	TextureManager::GetInstance()->Add("tex-ui", L"Resource/Xml/Textures/Misc/UI/UI_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	SpriteManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/UI/UiDB.xml");
	AnimationManager::GetInstance()->LoadFromXml("Resource/Xml/Textures/Misc/UI/UiAnim.xml");
	
	SceneManager::GetInstance()->AddScene("demo", new DemoScene());

	SceneManager::GetInstance()->ActiveScene("demo");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGame::GetInstance()->Init(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE);
	Events::GetInstance();
	LoadResources();
	CGame::GetInstance()->Run(MAX_FRAME_RATE);

	return 0;
}