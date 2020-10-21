#include "MarioGame.h"
#include "DemoScene.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"

MarioGame::MarioGame() {
	GameProperties properties = new CGameProperties();
	Init(properties);
}

void MarioGame::LoadResources()
{
	TextureManager::GetInstance()->Add("tex-mario", L"Resource/Xml/Textures/Mario/Mario_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Mario/MarioDB.xml");
	AnimationManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Mario/MarioAnim.xml");

	TextureManager::GetInstance()->Add("tex-enemy", L"Resource/Xml/Textures/Enemy/enemy_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Enemy/EnemyDB.xml");
	AnimationManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Enemy/EnemyAnim.xml");

	TextureManager::GetInstance()->Add("tex-intro", L"Resource/Xml/Textures/Misc/Intro/intro_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/Intro/IntroDB.xml");
	AnimationManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/Intro/IntroAnim.xml");

	TextureManager::GetInstance()->Add("tex-misc", L"Resource/Xml/Textures/Misc/Misc (Goods_Item)/misc_x3.png", D3DCOLOR_ARGB(0, 255, 255, 255));
	SpriteManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/Misc (Goods_Item)/MiscDB.xml");
	AnimationManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/Misc (Goods_Item)/MiscAnim.xml");

	TextureManager::GetInstance()->Add("tex-ui", L"Resource/Xml/Textures/Misc/UI/UI_x3.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	SpriteManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/UI/UiDB.xml");
	AnimationManager::GetInstance()->ImportFromXml("Resource/Xml/Textures/Misc/UI/UiAnim.xml");

	SceneManager::GetInstance()->AddScene("demo", make_shared<DemoScene>());
	SceneManager::GetInstance()->ActiveScene("demo");
}

void MarioGame::Update()
{
	SceneManager::GetInstance()->Update();
}

void MarioGame::Draw()
{
	graphic->Clear(D3DCOLOR_XRGB(255, 255, 255));
	SceneManager::GetInstance()->Render();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(new MarioGame())->Run();
	return 0;
}