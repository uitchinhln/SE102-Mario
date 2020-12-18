#include "Spawner.h"
#include "MEntityType.h"

shared_ptr<Spawner> Spawner::CreateSpawner(Vec2 fixedPos, MapProperties properties)
{
	shared_ptr<Spawner> spawner = make_shared<Spawner>();
	spawner->SetPosition(Vec2(fixedPos.x, fixedPos.y));
	return spawner;
}

Spawner::Spawner()
{
}

void Spawner::PositionUpdate()
{
}

void Spawner::PositionLateUpdate()
{
}

bool Spawner::HasCollideWith(DWORD id)
{
	return false;
}

void Spawner::FinalUpdate()
{
}

void Spawner::Update()
{
}

void Spawner::Render()
{
}

ObjectType Spawner::GetObjectType()
{
	return MEntityType::Spawner;
}

RectF Spawner::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Spawner::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Spawner::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}
