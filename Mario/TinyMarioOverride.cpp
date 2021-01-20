#include "TinyMario.h"

void TinyMario::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
}

void TinyMario::PositionUpdate()
{
}

void TinyMario::RestoreCollision()
{
}

void TinyMario::PositionLateUpdate()
{
}

void TinyMario::CollisionDoubleFilter()
{
}

void TinyMario::StatusUpdate()
{
}

void TinyMario::FinalUpdate()
{
}

bool TinyMario::HasCollideWith(DWORD id)
{
    return false;
}

bool TinyMario::IsGetThrough(GameObject& object, Direction direction)
{
    return true;
}

float TinyMario::GetDamageFor(GameObject& object, Direction direction)
{
    return 0.0f;
}

ObjectType TinyMario::GetObjectType()
{
    return MEntityType::TinyMario;
}