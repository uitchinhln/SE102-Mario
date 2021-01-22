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

	void Render(D3DCOLOR overlay) override;

	virtual void SpawnEntity(shared_ptr<GameObject> entity) override;

	virtual void SpawnEntity(shared_ptr<GameObject> entity, MapProperties& props) override;

	virtual vector<shared_ptr<GameObject>> GetUpdatingObjects() override;

	virtual void SetSceneContentPath(string path) override;

	virtual ObjectType GetSceneType() override;


	virtual void MapReadEvent(MapProperties& props);

	virtual void ObjectLoadEvent(const char* type, Vec2 fixedPos, Vec2 size, MapProperties& props);


	virtual void OnKeyDown(int key) override;

	virtual void OnKeyUp(int key) override;

	~PlayScene();
protected:
	vector<shared_ptr<GameObject>> objectList;

	Grid* grid;

	Vec2 camSize = Vec2(769, 579);

	virtual void HookEvent();

	virtual void UnhookEvent();

private:
	shared_ptr<Mario> mario;
};

