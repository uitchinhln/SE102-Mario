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
#include "IntroScene.h"

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
}

void MarioGame::LoadResources()
{
	TiXmlDocument doc("Resource/GameData.xml");

	mario = make_shared<Mario>();
	mario->SetKeyboard(make_shared<MarioKeyboard>());
	mario->SetCollisionCalculator(make_shared<CollisionCalculator>(mario));
	mario->SetPowerUp(make_shared<Small>(mario));

	this->tinyMario = make_shared<TinyMario>();
	this->tinyMario->HookEvent();

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

		string startId = scenes->Attribute("start");
		for (TiXmlElement* node = scenes->FirstChildElement("Scene"); node != NULL; node = node->NextSiblingElement("Scene")) {
			string id = node->Attribute("id");
			string type = node->Attribute("type");
			string path = node->Attribute("path");

			if (type.compare("PlayScene") == 0) {
				shared_ptr<PlayScene> scene = make_shared<PlayScene>();
				scene->SetSceneContentPath(path);

				SceneManager::GetInstance()->AddScene(id, scene);

				if (startId.compare(id) == 0) {
					state = GameState::WORLD_START;
				}
			}

			if (type.compare("WorldScene") == 0) {
				shared_ptr<WorldScene> scene = make_shared<WorldScene>();
				scene->SetSceneContentPath(path);

				SceneManager::GetInstance()->AddScene(id, scene);

				if (startId.compare(id) == 0) {
					state = GameState::GAME_WORLDMAP;
				}
			}

			if (type.compare("IntroScene") == 0) {
				shared_ptr<IntroScene> scene = make_shared<IntroScene>();
				scene->SetSceneContentPath(path);

				SceneManager::GetInstance()->AddScene(id, scene);

				if (startId.compare(id) == 0) {
					state = GameState::GAME_INTRO;
				}
			}
		}
		SceneManager::GetInstance()->ActiveScene(startId);

		doc.Clear();
	}
}

void MarioGame::Update()
{
	main->Update();
}

void MarioGame::Draw()
{
	main->Render();
}

void MarioGame::OnKeyUp(int key)
{
	main->OnKeyUp(key);
}

void MarioGame::OnKeyDown(int key)
{
	main->OnKeyDown(key);
}

shared_ptr<PlayerData> MarioGame::GetPlayerData()
{
	return playerdata;
}

shared_ptr<Mario> MarioGame::GetMario()
{
	return mario;
}

shared_ptr<TinyMario> MarioGame::GetTinyMario()
{
	return tinyMario;
}

FightProcessor* MarioGame::GetFightProcessor()
{
	return fightProc;
}

AchievementProcessor* MarioGame::GetAchievementProcessor()
{
	return achievementProc;
}

GameState MarioGame::GetGameState()
{
	return state;
}

MainUI* MarioGame::GetMainUI()
{
	return main;
}

void MarioGame::SetGameState(GameState value)
{
	if (this->state != value) {
		main->OnGameStateChange(__FILE__, this->state, value);
	}
	this->state = value;
}

MarioGame* MarioGame::GetInstance()
{
	return __instance;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MarioGame* game = new MarioGame();
	game->LoadResources();
	game->Run();
	return 0;
} 
