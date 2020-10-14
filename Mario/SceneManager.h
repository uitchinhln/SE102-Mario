#pragma once
#include "Utils.h"
#include "Scene.h"


class SceneManager
{
	static SceneManager* __instance;
	SceneManager();

	Scene current;

public:


	static SceneManager* GetInstance();
	~SceneManager();
};

