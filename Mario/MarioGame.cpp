#include "MarioGame.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "Mario.h"
#include "FontManager.h"

MarioGame::MarioGame() : CGame(new CGameProperties())
{
	this->LoadResources();
}

void MarioGame::LoadResources()
{
	TiXmlDocument doc("Resource/GameData.xml");

	SceneManager::GetInstance()->SetPlayer(make_shared<Mario>());

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

		/*for (TiXmlElement* node = resources->FirstChildElement("Resource"); node !=nullptr; node = node->NextSiblingElement("Resource"))
		{
			string textureId = node->Attribute("textureId");
			string texturesPath = node->FirstChildElement("Texture")->Attribute("path");
			string spritesPath = node->FirstChildElement("SpriteDB")->Attribute("path");
			string animationsPath = node->FirstChildElement("AnimationDB")->Attribute("path");

			string transColor;
			if (!node->FirstChildElement("Texture")->Attribute("transparent")) {
				transColor = "0,255,255,255";
			}
			else {
				transColor = node->FirstChildElement("Texture")->Attribute("transparent");
			}
			vector<string> argb = split(transColor, ",");

			TextureManager::GetInstance()->Add(textureId, ToLPCWSTR(texturesPath), D3DCOLOR_ARGB(stoi(argb[0]), stoi(argb[1]), stoi(argb[2]), stoi(argb[3])));
			SpriteManager::GetInstance()->ImportFromXml(textureId, spritesPath.c_str());
			AnimationManager::GetInstance()->ImportFromXml(textureId, animationsPath.c_str());
		}*/

		TiXmlElement* scenes = root->FirstChildElement("GameContent")->FirstChildElement("Scenes");

		for (TiXmlElement* node = scenes->FirstChildElement("Scene"); node != NULL; node = node->NextSiblingElement("Scene")) {
			string id = node->Attribute("id");
			string type = node->Attribute("type");

			if (type == "PlayScene") {
				shared_ptr<PlayScene> scene = make_shared<PlayScene>();
				scene->Load();
				scene->LoadFromXml(node);

				SceneManager::GetInstance()->AddScene(id, scene);
			}
		}

		string startId = scenes->Attribute("start");
		SceneManager::GetInstance()->ActiveScene(startId);
	}
	FontManager::GetInstance()->HardTest();
}

void MarioGame::Update()
{
	//auto start = std::chrono::high_resolution_clock::now();
	SceneManager::GetInstance()->Update();
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Render: \t%d\n", std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
}

void MarioGame::Draw()
{
	SceneManager::GetInstance()->Render();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(new MarioGame())->Run();
	return 0;
} 