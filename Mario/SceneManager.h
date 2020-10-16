#pragma once
#include "Utils.h"
#include "Scene.h"


class SceneManager
{
	static SceneManager* __instance;
	SceneManager();

	unordered_map<string, Scene> scenes;

	string activeSceneID;

public:
	void AddScene(string id, Scene scene);

	bool ActiveScene(string id);

	Scene GetActiveScene();

	static SceneManager* GetInstance();
	~SceneManager();
};

