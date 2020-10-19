#include "SpriteManager.h"
#include "TextureManager.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

SpriteManager* SpriteManager::__instance = nullptr;

Sprite SpriteManager::Add(string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	Sprite sprite = new CSprite(id, left, top, width, height, tex);
	sprites[id] = sprite;
	return sprite;
}

Sprite SpriteManager::Get(string id)
{
	if (sprites.find(id) == sprites.end()) return nullptr;
	return sprites[id];
}

void SpriteManager::ImportFromXml(const char* filePath)
{
	TiXmlDocument doc(filePath);
	if (doc.LoadFile())
	{
		TiXmlElement* pRoot = doc.RootElement();

		if (pRoot)
		{
			TiXmlElement* pSprites = pRoot->FirstChildElement();
			LPDIRECT3DTEXTURE9 texture = TextureManager::GetInstance()->Get(pSprites->Attribute("textureId"));

			for (TiXmlElement* pNode = pSprites->FirstChildElement(); pNode != nullptr; pNode = pNode->NextSiblingElement()) {
				string id = pNode->Attribute("id");

				int left = 0; pNode->QueryIntAttribute("left", &left);
				int top = 0; pNode->QueryIntAttribute("top", &top);
				int width = 0; pNode->QueryIntAttribute("width", &width);
				int height = 0; pNode->QueryIntAttribute("height", &height);

				//SpriteManager::GetInstance()->Add(id, left, top, width, height, texture);
				SpriteManager::GetInstance()->Add(id, left * 3, top * 3, width * 3, height * 3, texture);

				DebugOut(L"Load sprite: %s\n", ToLPCWSTR(id));
			}
		}
	}
}

SpriteManager* SpriteManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new SpriteManager();
	}

	return __instance;
}

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}
