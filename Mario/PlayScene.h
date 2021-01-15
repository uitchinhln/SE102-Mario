#pragma once
#include "Scene.h"
#include "Mario.h"
#include "MapProperties.h"
#include "Hud.h"
#include "Grid.h"

[event_receiver(native)]
class PlayScene :
    public CScene
{
public:
	void Load() override;

	void Unload() override;

	void Update() override;

	void Render() override;

	virtual void SpawnEntity(shared_ptr<GameObject> entity) override;

	virtual void SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props) override;

	virtual vector<shared_ptr<GameObject>> GetMovingObjects() override;

	virtual void OnKeyDown(int key) override;

	virtual void OnKeyUp(int key) override;

	virtual void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);

	virtual void MapReadEvent(MapProperties& props);

	~PlayScene();
protected:
	vector<shared_ptr<GameObject>> staticObjects;
	vector<shared_ptr<GameObject>> movingObjects;

	Grid* movingObjectGrid;
	Grid* staticObjectGrid;

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

