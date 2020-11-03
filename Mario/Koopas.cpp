#include "Koopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"

Koopas::Koopas()
{
	this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-move")->Clone();
	this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
	this->animations["RunningShell"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-shell-run")->Clone();
}

void Koopas::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
}

void Koopas::StatusUpdate()
{
}

void Koopas::Update()
{
}

void Koopas::FinalUpdate()
{
}

void Koopas::Render()
{
}

ObjectType Koopas::GetObjectType()
{
	return MEntityType::Koopas;
}

RectF Koopas::GetHitBox()
{
	return RectF();
}

bool Koopas::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float Koopas::GetDamageFor(IColliable& object, Direction direction)
{
	return 0.0f;
}
