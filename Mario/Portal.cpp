#include "Portal.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MarioGame.h"

Portal::Portal()
{
}

void Portal::Update()
{
	collidable = false;
	hasMario = CollisionCalculator::CommonAABB(hitbox, MarioGame::GetInstance()->GetMario()->GetHitBox());
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
}

bool Portal::HasCollideWith(DWORD id)
{
	return false;
}

void Portal::Render(D3DXCOLOR overlay)
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
