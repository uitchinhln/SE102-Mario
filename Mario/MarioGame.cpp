#include "MarioGame.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "Mario.h"

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

		this->LoadDefaultFont();
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