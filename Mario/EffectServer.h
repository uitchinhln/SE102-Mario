#pragma once
#include "Utils.h"
#include "IEffect.h"

class EffectServer
{
private:
	static EffectServer* __instance;
	EffectServer();

protected:
	vector<shared_ptr<IEffect>> effects;

public:

	void Update();

	void Render();

	void SpawnEffect(shared_ptr<IEffect> effect);

	static EffectServer* GetInstance();
};



