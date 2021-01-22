#include "Small.h"
#include "BigMario.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "GameObject.h"

Small::Small(shared_ptr<Mario> mario) : MarioPower(mario)
{
	size = Vec2(36, 45);
	sizeFixed = Vec2(36, 45);
}

void Small::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {

		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-small-mario-idle")->Clone();
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-small-mario-walk")->Clone();
		this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-small-mario-run")->Clone();
		this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-small-mario-run")->Clone();
		this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Float"] = AnimationManager::GetInstance()->Get("ani-small-mario-jump")->Clone();
		this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-small-mario-skid")->Clone();
		this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-small-mario-kick")->Clone();
		this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-small-mario-idle")->Clone();

		this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold")->Clone();
		this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-small-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = AnimationManager::GetInstance()->Get("ani-small-mario-idle-front")->Clone();
		this->animations["TeleHor"] = AnimationManager::GetInstance()->Get("ani-small-mario-walk")->Clone();
	}
}

void Small::CrouchUpdate()
{
}

ObjectType Small::GetMarioType()
{
	return MEntityType::SmallMario;
}

void Small::OnDamaged(float damage)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (damage > 0) {
			m->OnDeath();
		}
	}
}

void Small::OnPowerUp(ObjectType powerType)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 fixPos = Vec2(m->GetHitBox().left, m->GetHitBox().bottom);

		m->SetPowerUp(make_shared<BigMario>(m));

		m->GetPosition().x = fixPos.x;
		m->GetPosition().y = fixPos.y - (m->GetHitBox().bottom - m->GetHitBox().top);
	}
}
