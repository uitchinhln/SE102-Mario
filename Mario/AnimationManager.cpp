#include "AnimationManager.h"

AnimationManager* AnimationManager::__instance = NULL;

void AnimationManager::Add(string id, Animation animation)
{
	this->animations[id] = animation;
}

Animation AnimationManager::Get(string id)
{
	return animations[id];
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
