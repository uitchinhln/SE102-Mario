#include "MarioGame.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"

#include "MainUI.h"

#include "Mario.h"
#include "Small.h"

#include "PlayScene.h"
#include "WorldScene.h"

#include "FightProcessor.h"
#include "AchievementProcessor.h"

MarioGame* MarioGame::__instance = nullptr;

void MarioGame::LoadDefaultFont()
{
	DefaultFont = new Font();
	vector<FontSprite> fontSet;
	std::string prefix = "spr-font-";
	for (char c = '0'; c <= '9'; ++c)
		fontSet.push_back(FontSprite{ c, prefix + c });
	for (char c = 'A'; c <= 'Z'; ++c)
		fontSet.push_back(FontSprite{ c, prefix + c });
	fontSet.push_back(FontSprite{ '!', "spr-font-exclamation-point" });
	DefaultFont->Import(fontSet);
}

MarioGame::MarioGame() : CGame(new CGameProperties())
{
	__instance = this;

	this->main = new MainUI();	
	this->LoadResources();
}

void MarioGame::LoadResources()
{
	TiXmlDocument doc("Resource/GameData.xml");

	mario = make_shared<Mario>();
	mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	mario->SetPowerUp(make_shared<Small>(mario));

	playerdata = make_shared<PlayerData>();

	fightProc = new FightProcessor(playerdata);
	achievementProc = new AchievementProcessor(playerdata);

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		TiXmlElement* resources = root->FirstChildElement("Resources");

		TiXmlElement* textures = resources->FirstChildElement("Textures");
		TiXmlElement* sprites = resources->FirstChildElement("Sprites");
		TiXmlElement* animations = resources->FirstChildElement("Animations");


		for (TiXmlElement* node = textures->FirstChildElement("Texture"); node != nullptr; node = node->NextSiblingElement("Texture")) {
			TextureManager::GetInstance()->Add(node->Attribute("id"), ToLPCWSTR(node->Attribute("path")), D3DCOLOR_ARGB(0, 0, 0, 0));
		}

		for (TiXmlElement* node = sprites->FirstChildElement("Sprite"); node != nullptr; node = node->NextSiblingElement("Sprite")) {
			SpriteManager::GetInstance()->ImportFromXml(node->Attribute("path"));
		}

		for (TiXmlElement* node = animations->FirstChildElement("Animation"); node != nullptr; node = node->NextSiblingElement("Animation")) {
			AnimationManager::GetInstance()->ImportFromXml(node->Attribute("path"));
		}

		this->LoadDefaultFont();

		TiXmlElement* scenes = root->FirstChildElement("GameContent")->FirstChildElement("Scenes");

		this->main->CreateHUD(root->FirstChildElement("GameContent")->FirstChildElement("Hud"));

		for (TiXmlElement* node = scenes->FirstChildElement("Scene"); node != NULL; node = node->NextSiblingElement("Scene")) {
			string id = node->Attribute("id");
			string type = node->Attribute("type");
			string path = node->Attribute("path");

			if (type.compare("PlayScene") == 0) {
				shared_ptr<PlayScene> scene = make_shared<PlayScene>();
				scene->SetSceneContentPath(path);

				SceneManager::GetInstance()->AddScene(id, scene);
			}

			if (type.compare("WorldScene") == 0) {
				shared_ptr<WorldScene> scene = make_shared<WorldScene>();
				scene->SetSceneContentPath(path);

				SceneManager::GetInstance()->AddScene(id, scene);
			}
		}
		string startId = scenes->Attribute("start");
		SceneManager::GetInstance()->ActiveScene(startId);

		doc.Clear();
	}
}

void MarioGame::Update()
{
	playerdata->RemainingTime -= gameTime.ElapsedGameTime;
	main->Update();
}

void MarioGame::Draw()
{
	main->Render();
}

shared_ptr<PlayerData> MarioGame::GetPlayerData()
{
	return playerdata;
}

shared_ptr<Mario> MarioGame::GetMario()
{
	return mario;
}

FightProcessor* MarioGame::GetFightProcessor()
{
	return fightProc;
}

AchievementProcessor* MarioGame::GetAchievementProcessor()
{
	return achievementProc;
}

MarioGame* MarioGame::GetInstance()
{
	return __instance;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(new MarioGame())->Run();
	return 0;
} 
