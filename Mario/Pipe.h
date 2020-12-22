#pragma once
#include "GameObject.h"
#include "MapProperties.h"
#include "Sprite.h"

class Pipe :
    public GameObject
{
public:
	Pipe();

	virtual ObjectType GetObjectType() override;

	virtual RectF GetHitBox() override;

	virtual void CollisionDoubleFilter() override;

	virtual bool HasCollideWith(DWORD id) override;

	virtual bool IsGetThrough(GameObject& object, Direction direction) override;

	virtual float GetDamageFor(GameObject& object, Direction direction) override;

	virtual void Render() override;

	static shared_ptr<Pipe> CreatePipe(Vec2 pos, Vec2 size, MapProperties& props);

protected:
	Vec2 size;

	int length;

	float lengthOfPiece;

	Direction direction;

	Transform trans;

	Sprite head;
	Sprite body;

	RectF hitbox;
};

