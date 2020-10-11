#pragma once
#include "libs.h"
#include "Animation.h"

class AnimationManager
{
	AnimationManager() {}
	static AnimationManager* __instance;
	
	unordered_map<string, Animation> animations;

public:
	void Add(string id, Animation animation);
	
	Animation Get(string id);

	static AnimationManager* GetInstance();
	~AnimationManager();
};

