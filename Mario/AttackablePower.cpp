#include "AttackablePower.h"
#include "Mario.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "GameObject.h"
#include "Game.h"

AttackablePower::AttackablePower(shared_ptr<Mario> mario) : MarioPower(mario)
{
}

void AttackablePower::AttackUpdate()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();

	if (shared_ptr<Mario> m = mario.lock()) {
		if (m->GetMovingState() == MovingStates::CROUCH) {
			attackTimer.Stop();
			OnAttackDetroyed();
			return;
		}

		if (IsAttacking()) {
			if (attackTimer.Elapsed() < 1) {
				OnAttackStart();
			}
			if (attackTimer.Elapsed() >= MARIO_ATTACK_DURATION) {
				attackTimer.Stop();
				OnAttackFinish();
			}
			else {
				InAttackProgress();
			}
		}
		else {
			if (keyboard.IsKeyDown(DIK_Z) && !IsAttacking() && CanAttack()) {
				attackTimer.Restart();
				OnAttackStart();
			}
		}
		
	}
}

void AttackablePower::MoveProcess()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();

	if (shared_ptr<Mario> m = mario.lock()) {
		if (keyboard.IsKeyDown(DIK_LEFT) || keyboard.IsKeyDown(DIK_RIGHT)) {
			int keySign = keyboard.IsKeyDown(DIK_LEFT) ? -1 : 1;
			if (keySign * m->GetFacing() < 0 && IsAttacking()) {
				return;
			}			
		}
		MarioPower::MoveProcess();
	}
}

void AttackablePower::OnAttackDetroyed()
{
}

bool AttackablePower::CanAttack()
{
	return true;
}

bool AttackablePower::IsAttacking()
{
    return attackTimer.IsRunning();
}

void AttackablePower::Update()
{
	AttackUpdate();
	MarioPower::Update();
}

void AttackablePower::Render(D3DCOLOR overlay)
{
	this->InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	if (shared_ptr<Mario> m = mario.lock()) {
		JumpAnimation();
		MoveAnimation();

		if (IsAttacking() && m->GetMovingState() != MovingStates::CROUCH) {
			selectedAnimation = animations["Attack"];
			selectedAnimation->GetTransform()->Scale = Vec2((float)m->GetFacing(), 1);
			selectedAnimation->GetTransform()->Position = m->GetPosition() - cam + sizeFixed / 2;
			selectedAnimation->Render(attackTimer.Elapsed(), MARIO_ATTACK_DURATION, overlay);
			return;
		}

		if (m->GetMovingState() == MovingStates::CROUCH) {
			selectedAnimation = animations["Crouch"];
		}

		if (m->GetWarpState() != WarpStates::NONE) {
			selectedAnimation = m->GetWarpState() == WarpStates::HORIZONTAL ? animations["TeleHor"] : animations["TeleVer"];
		}

		if (m->Invulnerable() > 0 && (m->Invulnerable() / 50) % 2) {
			overlay = D3DCOLOR_ARGB(127, 255, 255, 255);
		}

		if (selectedAnimation == nullptr) return;
		selectedAnimation->SetPlayScale(max(0.4f, min(abs(m->GetVelocity().x) / MARIO_WALK_SPEED, 4)) * 1.5f);
		selectedAnimation->GetTransform()->Scale = Vec2((float)m->GetFacing(), 1);
		selectedAnimation->GetTransform()->Position = m->GetPosition() - cam + sizeFixed / 2;;
		selectedAnimation->Render(overlay);
	}
}
