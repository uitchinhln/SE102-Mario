#include "SpriteManager.h"
#include "TextureManager.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

SpriteManager* SpriteManager::__instance = nullptr;

Sprite SpriteManager::Add(string id, int left, int top, int width, int height, int xpivot, int ypivot, LPTEXTURE tex)
{
	Sprite sprite = new CSprite(id, left, top, width, height, xpivot, ypivot, tex);
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
			for (TiXmlElement* sNode = pRoot->FirstChildElement("Textures"); sNode != NULL ; sNode = sNode->NextSiblingElement("Textures"))
			{
				string textureId = sNode->Attribute("textureId");
				LPTEXTURE texture = TextureManager::GetInstance()->Get(textureId);

				for (TiXmlElement* pNode = sNode->FirstChildElement(); pNode != nullptr; pNode = pNode->NextSiblingElement()) {
					string id = pNode->Attribute("id");

					int left = 0, top = 0, width = 0, height = 0, xPivot = 0, yPivot = 0;
					pNode->QueryIntAttribute("left", &left);
					pNode->QueryIntAttribute("top", &top);
					pNode->QueryIntAttribute("width", &width);
					pNode->QueryIntAttribute("height", &height);
					if (pNode->Attribute("xPivot") != NULL) pNode->QueryIntAttribute("xPivot", &xPivot);
					if (pNode->Attribute("yPivot") != NULL) pNode->QueryIntAttribute("yPivot", &yPivot);

					//SpriteManager::GetInstance()->Add(id, left, top, width, height, texture);
					SpriteManager::GetInstance()->Add(id, left * 3, top * 3, width * 3, height * 3, xPivot * 3, yPivot * 3, texture);

					DebugOut(L"Load sprite: %s\n", ToLPCWSTR(id));
				}
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
