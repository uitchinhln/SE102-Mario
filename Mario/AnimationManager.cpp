#include "AnimationManager.h"

AnimationManager* AnimationManager::__instance = NULL;

void AnimationManager::Add(string id, Animation animation)
{
	this->animations[id] = animation;
}

Animation AnimationManager::Get(string id)
{
	if (animations.find(id) == animations.end()) return nullptr;
	return animations[id];
}

void AnimationManager::LoadFromXml(const char* filePath)
{
	TiXmlDocument doc(filePath);
	if (doc.LoadFile())
	{
		TiXmlElement* pRoot = doc.RootElement();

		if (pRoot)
		{
			TiXmlElement* pObj = pRoot->FirstChildElement();
			LPDIRECT3DTEXTURE9 texture = TextureManager::GetInstance()->Get(pObj->Attribute("textureId"));

			DebugOut(L"Object ID: %s\n", ToLPCWSTR(pObj->Attribute("gameObjectId")));
			DebugOut(L"Texture ID: %s\n", ToLPCWSTR(pObj->Attribute("textureId")));

			for (TiXmlElement* aniNode = pObj->FirstChildElement(); aniNode != nullptr; aniNode = aniNode->NextSiblingElement()) {
				int animationDefaultFrameTime = 100;
				aniNode->QueryIntAttribute("frameTime", &animationDefaultFrameTime);

				Animation ani = new CAnimation(animationDefaultFrameTime);

				for (TiXmlElement* spriteNode = aniNode->FirstChildElement(); spriteNode != nullptr; spriteNode = spriteNode->NextSiblingElement()) {
					string id = spriteNode->Attribute("id");
					int frameTime = 0; spriteNode->QueryIntAttribute("frameTime", &frameTime);
					Sprite sprite = SpriteManager::GetInstance()->Get(id);

					if (sprite == nullptr) {
						int left = 0; spriteNode->QueryIntAttribute("left", &left);
						int top = 0; spriteNode->QueryIntAttribute("top", &top);
						int width = 0; spriteNode->QueryIntAttribute("width", &width);
						int height = 0; spriteNode->QueryIntAttribute("height", &height);

						sprite = SpriteManager::GetInstance()->Add(id, left, top, width, height, texture);
					}

					ani->AddFrame(sprite, frameTime);
				}

				this->Add(aniNode->Attribute("aniId"), ani);
				DebugOut(L"Animation loaded. Id = %s\n", ToLPCWSTR(aniNode->Attribute("aniId")));
			}
		}
	}
	else
	{
		DebugOut(L"Failed to load file \"%s\"\n", ToLPCWSTR(filePath));
	}
}

AnimationManager* AnimationManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new AnimationManager();
	}
	return __instance;
}

AnimationManager::~AnimationManager()
{
}
