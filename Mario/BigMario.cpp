#include "BigMario.h"
#include "MEntityType.h"
#include "AnimationManager.h"

BigMario::BigMario(shared_ptr<Mario> mario) : MarioPower(mario)
{
}

void BigMario::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-big-mario-idle")->Clone();
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-big-mario-walk")->Clone();
		this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-big-mario-run")->Clone();
		this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-big-mario-high-speed")->Clone();
		this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-big-mario-jump")->Clone();
		this->animations["HighJump"] = AnimationManager::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-big-mario-fall")->Clone();
		this->animations["Float"] = AnimationManager::GetInstance()->Get("ani-big-mario-high-jump")->Clone();
		this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-big-mario-skid")->Clone();
		this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-big-mario-crouch")->Clone();
		this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-big-mario-kick")->Clone();

		this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-big-mario-hold")->Clone();
		this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-big-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-big-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = AnimationManager::GetInstance()->Get("ani-big-mario-idle-front")->Clone();
		this->animations["TeleHor"] = AnimationManager::GetInstance()->Get("ani-big-mario-walk")->Clone();
	}
}

ObjectType BigMario::GetMarioType()
{
	return MEntityType::BigMario;
}
