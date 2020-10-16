#pragma once
#include "Utils.h"
#include "Animation.h"
#include "TextureManager.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

class AnimationManager
{
	AnimationManager() {}
	static AnimationManager* __instance;
	
	unordered_map<string, Animation> animations;

public:
	virtual void Add(string id, Animation animation);
	
	virtual Animation Get(string id);

	virtual void LoadFromXml(const char* filePath);

	static AnimationManager* GetInstance();

	~AnimationManager();
};
