#include "MarioGame.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "Mario.h"

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

		for (TiXmlElement* node = resources->FirstChildElement("Resource"); node !=nullptr; node = node->NextSiblingElement("Resource"))
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
		}

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

}

void MarioGame::Update()
{
	SceneManager::GetInstance()->Update();
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