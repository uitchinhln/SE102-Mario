#pragma once
#include "GameObject.h"
#include "MapProperties.h"
#include "StopWatch.h"

class Spawner : 
	public GameObject
{
public:
	Spawner();

	virtual shared_ptr<GameObject> GetEntity();

	virtual void Spawn();

	virtual void OnGetInCamera() override;

	virtual void Update() override;

	virtual void CollisionUpdate(vector<shared_ptr<GameObject>>* coObj) override;

	virtual void PositionUpdate() override;

	virtual void RestoreCollision() override;

	virtual void PositionLateUpdate() override;

	virtual void CollisionDoubleFilter() override;

	virtual void StatusUpdate() override;

	virtual void FinalUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void Render(D3DCOLOR overlay) override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Spawner> CreateSpawner(Vec2 fixedPos, MapProperties properties);

protected:
	Vec2 size = Vec2(1, 1);
	
	bool autoSpawn = false;
	
	vector<shared_ptr<GameObject>> entities;

	string type = "Goomba";

	int respawnLimit = 0;
	int spawnLimit = 0;

	long spawnDelay = 0;
	Stopwatch spawnTimer;
};

