#pragma once
#include "Scene.h"
#include "Mario.h"
#include "MapProperties.h"
#include "Hud.h"
#include "EffectServer.h"
#include "TinyMario.h"

[event_receiver(native)]
class WorldScene :
	public CScene
{
protected:


public:
	virtual void Load() override;

	virtual void Unload() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual void SetSceneContentPath(string path) override;



	virtual void OnObjectLoad(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);

	virtual void OnKeyDown(int key) override;

	virtual void OnKeyUp(int key) override;

protected:

	shared_ptr<Hud> hud;

	shared_ptr<PlayerData> data;

	Vec2 camPos = Vec2(1, 1);
	Vec2 camSize = Vec2(769, 579);

	Vec2 hudPos = Vec2(0, 577);
	Vec2 hudSize = Vec2(769, 142);

	EffectServer backgroundAnimations;

	virtual void HookEvent();

	virtual void UnhookEvent();

private:
	shared_ptr<TinyMario> tinyMario;
	shared_ptr<Mario> mario;
};

