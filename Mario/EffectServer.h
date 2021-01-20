#pragma once
#include "Utils.h"
#include "IEffect.h"

class EffectServer
{
private:
	static EffectServer* __instance;

protected:
	vector<shared_ptr<IEffect>> effects;

public:
	EffectServer();

	void Update();

	void Render(D3DCOLOR overlay);

	void SpawnEffect(shared_ptr<IEffect> effect);

	void Clear();

	static EffectServer* GetInstance();
};



