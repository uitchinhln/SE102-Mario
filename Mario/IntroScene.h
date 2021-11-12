#pragma once
#include "Scene.h"
#include "Mario.h"
#include "MapProperties.h"
#include "Hud.h"

class EffectServer;

class IntroScene :
    public CScene
{
public:
	IntroScene();

	void Load() override;

	void Unload() override;


	void OriginalUpdate();

	void Update() override;


	void SetBackgroundColor(D3DXCOLOR color);


	void OriginalRender(D3DXCOLOR overlay);

	void Render(D3DXCOLOR overlay) override;

	EffectServer* GetBackgroundEffect();

	virtual vector<shared_ptr<GameObject>> GetUpdatingObjects() override;

	virtual ObjectType GetSceneType() override;


	virtual void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);

protected:
	vector<shared_ptr<GameObject>> objectList;

	Vec2 camSize = Vec2(769, 721);

	EffectServer* backgroundEffect;
};

