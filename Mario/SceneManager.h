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

	shared_ptr<CScene> GetScene(string id);

	void Update();
	
	void Render(D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	void OnKeyDown(int key);

	void OnkeyUp(int key);

	static SceneManager* GetInstance();
	~SceneManager();
};
