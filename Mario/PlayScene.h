#pragma once
#include "Scene.h"
#include "Mario.h"
#include "MapProperties.h"

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

	virtual void ColliableTilePreLoadEvent(const char* type, int id, shared_ptr<ColliableTile>& tile, MapProperties& props);

	virtual void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);

protected:
	vector<shared_ptr<IColliable>> mapObjects;

	virtual void HookEvent();

	virtual void UnhookEvent();

	Vec2 camPos = Vec2(0, 735);
	Vec2 camSize = Vec2(768, 579);

private:
	shared_ptr<Mario> mario;
};

