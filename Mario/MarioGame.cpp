#include "MarioGame.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"

MarioGame::MarioGame() : CGame(new CGameProperties())
{
	LoadResources();
}

void MarioGame::LoadResources()
{
	TiXmlDocument doc("Resource/GameData.xml");

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		TiXmlElement* resources = root->FirstChildElement("Resources");

		for (TiXmlElement* node = resources->FirstChildElement("Resource"); node !=nullptr; node = node->NextSiblingElement("Resource"))
		{
			string textureId = node->Attribute("textureId");
			string texturesPath = node->FirstChildElement("Texture")->Attribute("path");
			string spritesPath = node->FirstChildElement("SpriteDB")->Attribute("path");
			string animationsPath = node->FirstChildElement("AnimationDB")->Attribute("path");

			TextureManager::GetInstance()->Add(textureId, ToLPCWSTR(texturesPath), D3DCOLOR_ARGB(0, 255, 255, 255));
			SpriteManager::GetInstance()->ImportFromXml(textureId, spritesPath.c_str());
			AnimationManager::GetInstance()->ImportFromXml(textureId, animationsPath.c_str());
		}
		

		TiXmlElement* objects = root->FirstChildElement("GameContent")->FirstChildElement("Objects");

	}

}

void MarioGame::Update()
{
	SceneManager::GetInstance()->Update();
}

void MarioGame::Draw()
{
	graphic->Clear(D3DCOLOR_XRGB(181, 235, 242));
	SceneManager::GetInstance()->Render();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(new MarioGame())->Run();
	return 0;
}