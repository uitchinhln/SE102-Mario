#pragma once
#include "Utils.h"
#include "Scene.h"


class SceneManager
{
	static SceneManager* __instance;
	SceneManager();

	unordered_map<string, shared_ptr<CScene>> scenes;

	string activeSceneID;

public:
	void AddScene(string id, shared_ptr<CScene> scene);

	bool ActiveScene(string id);

	shared_ptr<CScene> GetActiveScene();

	void Update();
	
	void Render();

	static SceneManager* GetInstance();
	~SceneManager();
};

