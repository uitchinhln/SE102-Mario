#include "BigMario.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "Small.h"
#include "GameEvent.h"
#include "ScoreFX.h"
#include "FireMario.h"
#include "RaccoonMario.h"

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

void BigMario::OnDamaged(float damage)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (damage >= 2) {
			m->OnDeath();
			return;
		}

		Vec2 fixPos = Vec2(m->GetHitBox().left, m->GetHitBox().bottom);

		if (damage >= 1) {
			m->SetPowerUp(make_shared<Small>(m));
		}

		m->GetPosition().x = fixPos.x;
		m->GetPosition().y = fixPos.y - (m->GetHitBox().bottom - m->GetHitBox().top);
	}
}

void BigMario::OnPowerUp(ObjectType powerType)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (powerType == MEntityType::RedMushroom) {
			shared_ptr<IEffect> effect = make_shared<ScoreFX>(m->GetPosition(), Score::S1000);
			__raise (*GameEvent::GetInstance()).PlayerBonusEvent(__FILE__, effect, Score::S1000);
			return;
		}

		Vec2 fixPos = Vec2(m->GetHitBox().left, m->GetHitBox().bottom);

		if (powerType == MEntityType::RaccoonLeaf) {
			m->SetPowerUp(make_shared<RaccoonMario>(m));
		}
		else if (powerType == MEntityType::FireFlower) {
			m->SetPowerUp(make_shared<FireMario>(m));
		}

		m->GetPosition().x = fixPos.x;
		m->GetPosition().y = fixPos.y - (m->GetHitBox().bottom - m->GetHitBox().top);
	}
}
