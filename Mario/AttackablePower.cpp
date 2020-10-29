#include "AttackablePower.h"
#include "Mario.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"

AttackablePower::AttackablePower(shared_ptr<Mario> mario) : MarioPowerUp(mario)
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
		
		if (!CanAttack()) {
			return;
		}

		if (IsAttacking()) {
			if (attackTimer.Elapsed() >= MARIO_ATTACK_TIME) {
				attackTimer.Stop();
				OnAttackFinish();
			}
			else {
				InAttackProgress();
			}
		}
		else {
			if (keyboard.IsKeyDown(DIK_Z) && !IsAttacking()) {
				attackTimer.Restart();
			}
		}
		
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

void AttackablePower::Update(vector<shared_ptr<IColliable>>* coObj)
{
	AttackUpdate();
	MarioPowerUp::Update(coObj);
}

void AttackablePower::Render()
{
	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	if (shared_ptr<Mario> m = mario.lock()) {
		JumpAnimation();
		MoveAnimation();

		if (IsAttacking() && m->GetMovingState() != MovingStates::CROUCH) {
			selectedAnimation = animations["Attack"];
			selectedAnimation->GetTransform()->Scale = Vec2(m->GetFacing(), 1);
			selectedAnimation->GetTransform()->Position = m->GetPosition() - cam;
			selectedAnimation->Render(attackTimer.Elapsed(), MARIO_ATTACK_TIME);
			return;
		}

		if (m->GetMovingState() == MovingStates::CROUCH) {
			selectedAnimation = animations["Crouch"];
		}		

		selectedAnimation->SetPlayScale(max(1, min(abs(m->GetVelocity().x) / MARIO_WALK_SPEED, 4)) * 1.5);
		selectedAnimation->GetTransform()->Scale = Vec2(m->GetFacing(), 1);
		selectedAnimation->GetTransform()->Position = m->GetPosition() - cam;
		selectedAnimation->Render();
	}
}
