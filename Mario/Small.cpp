#include "Small.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "SceneManager.h"

Small::Small(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{
	this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-small-mario-idle")->Clone();
	this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-small-mario-walk")->Clone();
	this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-small-mario-run")->Clone();
	this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-small-mario-run")->Clone();
	this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
	this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
	this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
	this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-small-mario-skid")->Clone();
	this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-small-mario-kick")->Clone();

	this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold")->Clone();
	this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold-idle")->Clone();
	this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold-fall")->Clone();

	size = Vec2(36, 45);
	sizeFixed = Vec2(36, 45);
}

void Small::CrouchUpdate()
{
}

ObjectType Small::GetMarioType()
{
	return MEntityType::SmallMario;
}
