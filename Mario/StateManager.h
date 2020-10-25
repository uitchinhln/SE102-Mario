#pragma once
#include "Utils.h"
#include "MarioPowerUp.h"

class StateManager
{
public:
	static StateManager* GetInstance();
	
	void Add(string id, shared_ptr<MarioPowerUp> marioState);

	shared_ptr<MarioPowerUp> Get(string id);

private:
	static StateManager* __instance;
	StateManager();

	unordered_map<string, shared_ptr<MarioPowerUp>> contents;
};

