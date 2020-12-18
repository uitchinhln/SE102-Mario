#pragma once
#include "GameObject.h"
#include "MapProperties.h"

class Spawner : 
	public GameObject
{
public:
	Spawner();

	virtual void PositionUpdate() override;

	virtual void PositionLateUpdate() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual void FinalUpdate() override;

	virtual void Update() override;

	virtual void Render() override;

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	static shared_ptr<Spawner> CreateSpawner(Vec2 fixedPos, MapProperties properties);

protected:
	Vec2 size = Vec2(48, 48);
};

