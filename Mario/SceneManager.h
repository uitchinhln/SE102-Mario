#pragma once
#include "Utils.h"
#include "Scene.h"


class SceneManager
{
	static SceneManager* __instance;
	SceneManager();

	unordered_map<string, shared_ptr<CScene>> scenes;

	string activeSceneID;

	shared_ptr<GameObject> player;

public:
	void AddScene(string id, shared_ptr<CScene> scene);

	bool ActiveScene(string id);

	shared_ptr<CScene> GetActiveScene();

	template<class T>
	shared_ptr<T> GetPlayer();

	void SetPlayer(shared_ptr<GameObject> player);

	void Update();
	
	void Render();

	void OnKeyDown(int key);

	void OnkeyUp(int key);

	static SceneManager* GetInstance();
	~SceneManager();
};

template<class T>
inline shared_ptr<T> SceneManager::GetPlayer()
{
	return dynamic_pointer_cast<T>(player);
}
