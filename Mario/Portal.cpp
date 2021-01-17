#include "Portal.h"
#include "SceneManager.h"
#include "Mario.h"

Portal::Portal()
{
	this->mario = SceneManager::GetInstance()->GetPlayer<Mario>();
}

void Portal::Update()
{
	active = false;
	hasMario = CollisionCalculator::CommonAABB(hitbox, mario->GetHitBox());
}

void Portal::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{	
}

void Portal::PositionUpdate()
{
}

void Portal::RestoreCollision()
{
}

void Portal::PositionLateUpdate()
{
}

void Portal::CollisionDoubleFilter()
{
}

void Portal::StatusUpdate()
{
}

void Portal::FinalUpdate()
{
	active = true;
}

bool Portal::HasCollideWith(DWORD id)
{
	return false;
}

void Portal::Render()
{
}

RectF Portal::GetHitBox()
{
	return hitbox;
}

bool Portal::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Portal::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}
