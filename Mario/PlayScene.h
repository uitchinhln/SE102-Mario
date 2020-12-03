#pragma once
#include "Scene.h"
#include "Mario.h"
#include "MapProperties.h"
#include "Hud.h"

[event_receiver(native)]
class PlayScene :
    public CScene
{
public:
	void LoadFromXml(TiXmlElement* data);

	void Load() override;

	void Unload() override;

	void Update();

	void Render();

	virtual void OnKeyDown(int key) override;

	virtual void OnKeyUp(int key) override;

	virtual void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);

protected:
	vector<shared_ptr<GameObject>> mapObjects;

	shared_ptr<Hud> hud;

	virtual void HookEvent();

	virtual void UnhookEvent();

	Vec2 camPos = Vec2(0, 0);
	Vec2 camSize = Vec2(769, 579);

	Vec2 hudPos = Vec2(0, 579);
	Vec2 hudSize = Vec2(769, 142);

private:
	shared_ptr<Mario> mario;
};

