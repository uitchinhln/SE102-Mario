#include "FireMario.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MarioTailed.h"
#include "GameObject.h"
#include "Game.h"
#include "Small.h"
#include "BigMario.h"
#include "ScoreFX.h"
#include "GameEvent.h"
#include "RaccoonMario.h"

FireMario::FireMario(shared_ptr<Mario> mario) : AttackablePower(mario)
{
	this->MARIO_ATTACK_DURATION = 100;

	for (int i = 0; i < 2; i++) {
		shared_ptr<MarioFireBall> fireball = make_shared<MarioFireBall>(mario);
		fireball->SetCollisionCalculator(make_shared<CollisionCalculator>(fireball));
		fireball->SetActive(false);
		bulletPool.push_back(fireball);
	}
}

void FireMario::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-fire-mario-idle")->Clone();
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-fire-mario-walk")->Clone();
		this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-fire-mario-run")->Clone();
		this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-speed")->Clone();
		this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-fire-mario-jump")->Clone();
		this->animations["HighJump"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-fire-mario-fall")->Clone();
		this->animations["Float"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
		this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-fire-mario-skid")->Clone();
		this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-fire-mario-crouch")->Clone();
		this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-fire-mario-kick")->Clone();
		this->animations["Attack"] = AnimationManager::GetInstance()->Get("ani-fire-mario-throw")->Clone();

		this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-fire-mario-hold")->Clone();
		this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-fire-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-fire-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = AnimationManager::GetInstance()->Get("ani-fire-mario-idle-front")->Clone();
		this->animations["TeleHor"] = AnimationManager::GetInstance()->Get("ani-fire-mario-walk")->Clone();
	}
}

bool FireMario::CanAttack()
{
	for each (shared_ptr<MarioFireBall> bullet in bulletPool)
	{
		if (!bullet->IsActive()) {			
			return true;
		}
	}
	return false;
}

void FireMario::OnAttackStart()
{
	for each (shared_ptr<MarioFireBall> bullet in bulletPool)
	{
		if (!bullet->IsActive()) {
			bullet->Reset();
			SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(bullet);
			return;
		}
	}
}

void FireMario::OnAttackFinish()
{
	
}

void FireMario::InAttackProgress()
{
}

void FireMario::OnKeyDown(int key)
{
	AttackablePower::OnKeyDown(key);

	// Attack
	if (key == DIK_A && !IsAttacking() && CanAttack()) {
		attackTimer.Restart();
	}
}

void FireMario::OnDamaged(float damage)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (damage >= 3) {
			m->OnDeath();
			return;
		}

		Vec2 fixPos = Vec2(m->GetHitBox().left, m->GetHitBox().bottom);

		if (damage >= 2) {
			m->SetPowerUp(make_shared<Small>(m));
			m->SetInvulnerable(INVULNERABLE_TIME);
		}
		else if (damage >= 1) {
			m->SetPowerUp(make_shared<BigMario>(m));
			m->SetInvulnerable(INVULNERABLE_TIME);
		}

		m->GetPosition().x = fixPos.x;
		m->GetPosition().y = fixPos.y - (m->GetHitBox().bottom - m->GetHitBox().top);
	}
}

void FireMario::OnPowerUp(ObjectType powerType)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (powerType == MEntityType::FireFlower) {
			__raise (*GameEvent::GetInstance()).PlayerScoreChangeEvent(__FILE__, Score::S1000, m->GetPosition(), ScoreSource::POWERUP);
			return;
		}

		Vec2 fixPos = Vec2(m->GetHitBox().left, m->GetHitBox().bottom);

		if (powerType == MEntityType::RedMushroom) {
			m->SetPowerUp(make_shared<BigMario>(m));
		}
		else if (powerType == MEntityType::RaccoonLeaf) {
			m->SetPowerUp(make_shared<RaccoonMario>(m));
		}

		m->GetPosition().x = fixPos.x;
		m->GetPosition().y = fixPos.y - (m->GetHitBox().bottom - m->GetHitBox().top);
	}
}

ObjectType FireMario::GetMarioType()
{
    return MEntityType::FireMario;
}
