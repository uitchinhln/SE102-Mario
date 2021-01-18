#include "RaccoonMario.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "BigMario.h"
#include "MarioTailed.h"
#include "SceneManager.h"
#include "Koopas.h"
#include "GameObject.h"
#include "Game.h"

RaccoonMario::RaccoonMario(shared_ptr<Mario> mario) : AttackablePower(mario)
{
	this->MARIO_ATTACK_DURATION = 250;
	this->MARIO_SUPER_PUSH_FORCE = MARIO_FLYING_UP_FORCE;
	this->MARIO_SUPER_JUMP_HEIGHT = 80;
	this->MAX_FLY_SPEED = 0.405f;

	tail = make_shared<MarioTailed>(mario, MARIO_ATTACK_DURATION / 2);
	tail->SetCollisionCalculator(make_shared<CollisionCalculator>(tail));
}

void RaccoonMario::InitResource(bool force)
{
	if (this->animations.size() < 1 || force) {
		this->animations["Idle"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-idle")->Clone();
		this->animations["Walk"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-walk")->Clone();
		this->animations["Run"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-walk")->Clone();
		this->animations["HighSpeed"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-speed-up")->Clone();
		this->animations["Jump"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-jump")->Clone();
		this->animations["HighJump"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-jump")->Clone();
		this->animations["Fly"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-fly")->Clone();
		this->animations["Float"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-float")->Clone();
		this->animations["Fall"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-fall")->Clone();
		this->animations["Skid"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-skid")->Clone();
		this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-crouch")->Clone();
		this->animations["Kick"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-kick")->Clone();
		this->animations["Attack"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-spin")->Clone();

		this->animations["Hold"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-hold")->Clone();
		this->animations["HoldIdle"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-hold-idle")->Clone();
		this->animations["HoldFall"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-hold-fall")->Clone();

		this->animations["TeleVer"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-idle-front")->Clone();
		this->animations["TeleHor"] = AnimationManager::GetInstance()->Get("ani-raccoon-mario-walk")->Clone();
	}
}

void RaccoonMario::OnAttackStart()
{
	if (shared_ptr<Mario> m = mario.lock()) {	
		tail->Reset();
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntityWithoutGrid(tail);
	}
}

void RaccoonMario::OnAttackFinish()
{
}

void RaccoonMario::InAttackProgress()
{
}

bool RaccoonMario::MiniJumpDetect(bool forceX)
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		if (keyboard.IsKeyDown(DIK_X) || forceX) {
			if (m->GetPowerMeter() >= PMETER_MAX) {
				m->SetJumpingState(JumpingStates::SUPER_JUMP);
				m->SetOnGround(false);
				m->SetJumpBeginPosition(m->GetPosition().y); 

				SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetBoundingEdge(Direction::Top, 0.0f);

				if (!flyTimer.IsRunning()) {
					flyTimer.Restart();
				}
				return true;
			}

			if (m->IsOnGround()) {
				m->SetJumpingState(JumpingStates::JUMP);
				m->SetOnGround(false);
				m->SetJumpBeginPosition(m->GetPosition().y);
				return true;
			}

			floatEndTime = 0;
			if (m->GetJumpingState() == JumpingStates::FALLING || m->GetJumpingState() == JumpingStates::FLOATING) {
				floatEndTime = CGame::Time().TotalGameTime + dt;
				if (forceX) {
					floatEndTime += 90;
				}
			}
		}
		if (CGame::Time().TotalGameTime < floatEndTime) {
			m->SetJumpingState(JumpingStates::FLOATING);
			m->GetVelocity().y = MARIO_FLOATING_SPEED * dt;
		}
	}
	return false;
}

void RaccoonMario::PowerMeterProcess()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		float maxRun = abs(m->GetVelocity().x) > MARIO_RUN_SPEED * 0.85f;

		if (maxRun && m->IsOnGround())
			m->SetPowerMeter(max(0.0f, min(m->GetPowerMeter() + PMETER_UP_STEP * dt, PMETER_MAX + 0.2f)));
		else {
			if (flyTimer.IsRunning()) {
				if (flyTimer.Elapsed() >= 4000) {
					m->SetPowerMeter(0);
					flyTimer.Stop();
				}
			}
			else {
				if (m->GetPowerMeter() > 0)
					m->SetPowerMeter(max(0.0f, min(m->GetPowerMeter() - PMETER_DOWN_STEP * dt, PMETER_MAX)));
			}
		}
	}
}

void RaccoonMario::OnKeyDown(int key)
{
	AttackablePower::OnKeyDown(key);

	// Attack
	if (key == DIK_A && CanAttack() && !IsAttacking()) {
		attackTimer.Restart();
	}
}

ObjectType RaccoonMario::GetMarioType()
{
    return MEntityType::RaccoonMario;
}
