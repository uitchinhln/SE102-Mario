#include "RaccoonMario.h"
#include "AnimationManager.h"
#include "Mario.h"
#include "BigMario.h"
#include "MarioTailed.h"
#include "SceneManager.h"

RaccoonMario::RaccoonMario(shared_ptr<Mario> mario) : AttackablePower(mario)
{
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

	this->MARIO_ATTACK_TIME = 250;
}

void RaccoonMario::OnAttackStart()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		shared_ptr<MarioTailed> tail = make_shared<MarioTailed>(m, MARIO_ATTACK_TIME / 2);
		tail->SetCollisionCalculator(make_shared<CollisionCalculator>(tail));
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(tail);
	}
}

void RaccoonMario::OnAttackFinish()
{
}

void RaccoonMario::InAttackProgress()
{
}

void RaccoonMario::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();

	if (shared_ptr<Mario> m = mario.lock()) {

		m->GetVelocity().y += m->GetGravity() * dt;
		m->GetDistance() = m->GetVelocity() * dt;

		shared_ptr<CollisionCalculator> collisionCal = m->GetCollisionCalc();

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

		// No collision occured, proceed normally
		if (coResult.size() == 0)
		{
			//m->GetPosition() += m->GetDistance();
			m->SetOnGround(false);
			if (m->GetDistance().y > 0 && m->GetJumpingState() != JumpingStates::SUPER_JUMP && !(keyboard.IsKeyDown(DIK_S) || keyboard.IsKeyDown(DIK_X))) {
				m->SetJumpingState(JumpingStates::FALLING);
			}
		}
		else {
			//m->GetDistance() = collisionCal->GetNewDistance();
			Vec2 jet = collisionCal->GetJet();
			//m->GetPosition() += m->GetDistance();

			if (jet.x != 0) m->GetVelocity().x = 0;
			if (jet.y != 0) m->GetVelocity().y = 0;
		}

		//fixed position
		if (m->GetPosition().x < 0.3) {
			m->GetPosition().x = 0.3;
			m->GetVelocity().x = 0;
		}

		m->SetOnGround(false);
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (coll->SAABBResult.Direction == Direction::Top && !coll->GameColliableObject->IsGetThrough(*m, coll->SAABBResult.Direction)) {
				m->SetOnGround(true);
				if (m->GetJumpingState() != JumpingStates::SUPER_JUMP)
					m->SetJumpingState(JumpingStates::IDLE);
				m->GetGravity() = MARIO_GRAVITY;
			}
			if (MEntityType::IsEnemy(coll->GameColliableObject->GetObjectType())) {
				float damage = coll->GameColliableObject->GetDamageFor(*m, coll->SAABBResult.Direction);
				if (damage > 0) {
					
				}
				else {
					m->SetOnGround(true);
					m->SetJumpingState(JumpingStates::IDLE);
					MiniJumpDetect(true);
				}
			}
		}
	}
}

void RaccoonMario::MiniJumpDetect(bool forceX)
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;


	if (shared_ptr<Mario> m = mario.lock()) {
		if (m->GetJumpingState() == JumpingStates::FALLING || m->GetJumpingState() == JumpingStates::FLOATING) {
			m->SetJumpingState(JumpingStates::FALLING);
		}
		if (keyboard.IsKeyDown(DIK_X) || forceX) {
			if (m->GetJumpingState() == JumpingStates::SUPER_JUMP && m->GetPowerMeter() >= PMETER_MAX) {
				m->GetVelocity().y = -MARIO_JUMP_FORCE;
			}

			if (m->IsOnGround() && m->GetJumpingState() == JumpingStates::IDLE) {
				m->GetVelocity().y = -MARIO_JUMP_FORCE;
				m->SetJumpingState(JumpingStates::JUMP);
				m->SetOnGround(false);

				if (m->GetPowerMeter() >= PMETER_MAX) {
					m->SetJumpingState(JumpingStates::SUPER_JUMP);
					flyTimer.Restart();
					m->SetCanHighJump(true);
				}
			}

			if (m->GetJumpingState() == JumpingStates::FALLING || m->GetJumpingState() == JumpingStates::FLOATING) {
				m->SetJumpingState(JumpingStates::FLOATING);
				m->GetVelocity().y = MARIO_GRAVITY * 6 * dt;
				m->GetGravity() = MARIO_GRAVITY;
			}
		}
	}
}

void RaccoonMario::PowerMeterUpdate()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		float maxRun = abs(m->GetVelocity().x) > MARIO_RUN_SPEED * 0.85f;

		if (maxRun && m->IsOnGround())
			m->SetPowerMeter(max(0.0f, min(m->GetPowerMeter() + PMETER_UP_STEP * dt, PMETER_MAX + 1)));
		else {
			if (m->GetJumpingState() == JumpingStates::SUPER_JUMP) {
				if (flyTimer.Elapsed() >= 4000) {
					m->SetPowerMeter(0);
					m->SetJumpingState(JumpingStates::FLOATING);
					m->SetCanHighJump(false);
					flyTimer.Stop();
				}
			}
			else {
				if (m->GetPowerMeter() > 0)
					m->SetPowerMeter(max(0.0f, min(m->GetPowerMeter() - PMETER_DOWN_STEP * dt, PMETER_MAX)));
			}
		}
		//DebugOut(L"PMeter: %f\n", m->GetPowerMeter());
	}
}

void RaccoonMario::JumpAnimation()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		switch (m->GetJumpingState())
		{
		case JumpingStates::SUPER_JUMP:
			selectedAnimation = animations["Fly"];
			break;
		case JumpingStates::JUMP:
			selectedAnimation = animations["Jump"];
			break;
		case JumpingStates::HIGH_JUMP:
			selectedAnimation = animations["Jump"];
			break;
		case JumpingStates::FALLING:
			selectedAnimation = animations["Fall"];
			break;
		case JumpingStates::FLOATING:
			selectedAnimation = animations["Float"];
			break;
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
