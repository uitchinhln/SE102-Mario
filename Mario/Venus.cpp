#include "Venus.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Game.h"

Venus::Venus()
{
	this->renderOrder = 125;
	this->Gravity = 0;
	this->Velocity.x = 0;
}

void Venus::InitResource()
{
	if (animations.size() < 1) {
		this->animations["RevealHeadUp"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headup")->Clone();
		this->animations["RevealHeadDown"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headdown")->Clone();
		this->animations["IdleHeadUp"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headup-idle")->Clone();
		this->animations["IdleHeadDown"] = AnimationManager::GetInstance()->Get("ani-green-venus-fire-trap-headdown-idle")->Clone();
	}
}

void Venus::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	GetCollisionCalc()->CalcPotentialCollisions(coObj, false);
}

void Venus::RestoreCollision()
{
	//Do nothing
}

void Venus::PositionUpdate()
{
	Position += Distance;
}

void Venus::PositionLateUpdate()
{
	//Do nothing
}

void Venus::StatusUpdate()
{
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() != 0) {
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMarioWeapon(coll->Object->GetObjectType())) {
				float damage = coll->Object->GetDamageFor(*this, coll->SAABBResult.Direction);
				if (damage > 0) {
					SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
					break;
				}
			}
		}
	}
}

void Venus::Update()
{

}

void Venus::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	collisionCal->Clear();
}

void Venus::Render()
{

}

ObjectType Venus::GetObjectType()
{
	return MEntityType::Venus;
}

RectF Venus::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool Venus::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float Venus::GetDamageFor(GameObject& object, Direction direction)
{
	if (MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
		return 1.0f;
	}
	if (MEntityType::IsMarioWeapon(object.GetObjectType())) {
		return 1.0f;
	}
	return 0.0f;
}
