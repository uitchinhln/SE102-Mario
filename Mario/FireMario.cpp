#include "FireMario.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Mario.h"
#include "MarioTailed.h"

FireMario::FireMario(shared_ptr<Mario> mario) : AttackablePower(mario)
{
	this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-fire-mario-idle")->Clone();
	this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-fire-mario-walk")->Clone();
	this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-fire-mario-run")->Clone();
	this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-speed")->Clone();
	this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-fire-mario-jump")->Clone();
	this->animations["HighJump"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
	this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-fire-mario-high-jump")->Clone();
	this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-fire-mario-fall")->Clone();
	this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-fire-mario-skid")->Clone();
	this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-fire-mario-crouch")->Clone();
	this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-fire-mario-kick")->Clone();
	this->animations["Attack"] = AnimationManager::GetInstance()->Get("ani-fire-mario-throw")->Clone();

	this->MARIO_ATTACK_TIME = 100;

	for (int i = 0; i < 2; i++) {
		shared_ptr<MarioFireBall> fireball = make_shared<MarioFireBall>(mario);
		fireball->SetCollisionCalculator(make_shared<CollisionCalculator>(fireball));
		fireball->SetActive(false);
		bulletPool.push_back(fireball);
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
			SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(bullet);
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

ObjectType FireMario::GetMarioType()
{
    return MEntityType::FireMario;
}
