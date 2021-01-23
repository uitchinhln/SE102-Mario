#include "BigLuigi.h"
#include "AnimationManager.h"

BigLuigi::BigLuigi(shared_ptr<Mario> mario) : MarioPower(mario)
{
}

void BigLuigi::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-big-luigi-idle")->Clone();
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-big-luigi-walk")->Clone();
		this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-big-luigi-run")->Clone();
		this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-big-luigi-high-speed")->Clone();
		this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-big-luigi-jump")->Clone();
		this->animations["HighJump"] = AnimationManager::GetInstance()->Get("ani-big-luigi-high-jump")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-big-luigi-high-jump")->Clone();
		this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-big-luigi-fall")->Clone();
		this->animations["Float"] = AnimationManager::GetInstance()->Get("ani-big-luigi-high-jump")->Clone();
		this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-big-luigi-skid")->Clone();
		this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-big-luigi-crouch")->Clone();
		this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-big-luigi-kick")->Clone();

		this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-big-luigi-hold")->Clone();
		this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-big-luigi-hold-idle")->Clone();
		this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-big-luigi-hold-fall")->Clone();

		this->animations["TeleVer"] = AnimationManager::GetInstance()->Get("ani-big-luigi-idle-front")->Clone();
		this->animations["TeleHor"] = AnimationManager::GetInstance()->Get("ani-big-luigi-walk")->Clone();
	}
}

ObjectType BigLuigi::GetMarioType()
{
    return MEntityType::BigLuigi;
}

void BigLuigi::OnDamaged(float damage)
{
}

void BigLuigi::OnPowerUp(ObjectType powerType)
{
}
