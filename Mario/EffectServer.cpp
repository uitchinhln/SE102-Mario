#include "EffectServer.h"
#include "SceneManager.h"

EffectServer* EffectServer::__instance = nullptr;

EffectServer::EffectServer()
{
}

void EffectServer::Update()
{
	vector<shared_ptr<IEffect>> workspace;
	workspace.insert(workspace.end(), effects.begin(), effects.end());

	for each (shared_ptr<IEffect> fx in workspace)
	{
		fx->Update();
	}

	effects.erase(remove_if(effects.begin(), effects.end(), [](const shared_ptr<IEffect>& fx) {
		return !fx->Active;
		}), effects.end());
}

void EffectServer::Render(D3DXCOLOR overlay)
{
	for each (shared_ptr<IEffect> fx in effects)
	{
		fx->Render(overlay);
	}
}

void EffectServer::SpawnEffect(shared_ptr<IEffect> effect)
{
	if (effect && effect->Active) {
		effects.push_back(effect);
	}
}

void EffectServer::Clear()
{
	this->effects.clear();
}

EffectServer* EffectServer::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new EffectServer();
	}
	return __instance;
}
