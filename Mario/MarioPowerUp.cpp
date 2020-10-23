#include "MarioPowerUp.h"
#include "Mario.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"

MarioPowerUp::MarioPowerUp(shared_ptr<Mario> mario)
{
	this->mario = mario;
}

RectF MarioPowerUp::GetBoundingBox()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 pos = m->GetPosition();
		return RectF(pos.x, pos.y, pos.x + 15 * 3, pos.y + 27 * 3);
	}
	return RectF(0, 0, 0, 0);
}

void MarioPowerUp::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	if (shared_ptr<Mario> m = mario.lock()) {

		shared_ptr<CollisionCalculator> collisionCal = m->GetCollisionCalc();

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

		// No collision occured, proceed normally
		if (coResult.size() == 0)
		{
			m->GetPosition() += m->GetDistance();
		}
		else {
			m->GetDistance() = collisionCal->GetNewDistance();
			Vec2 jet = collisionCal->GetJet();
			m->GetPosition() += m->GetDistance();

			if (jet.x != 0) m->GetVelocity().x = 0;
			if (jet.y != 0) m->GetVelocity().y = 0;
		}

		//fixed position
		if (m->GetPosition().x < 0.3) {
			m->GetPosition().x = 0.3;
			m->GetVelocity().x = 0;
		}
		if (m->GetPosition().y < 0.3) {
			m->GetPosition().y = 0.3;
			m->GetVelocity().y = 0;
		}

		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (coll->SAABBResult.Direction == Direction::Top && !m->IsGetThrough(*coll->GameColliableObject, coll->SAABBResult.Direction)) {
				m->SetOnGround(true);
				break;
			}
			else {
				m->SetOnGround(false);
			}
		}
	}
}

void MarioPowerUp::MovementUpdate()
{
	if (shared_ptr<Mario> m = mario.lock()) {

		KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
		DWORD dt = CGame::GetInstance()->Time().ElapsedGameTime;

		//Horizontal process
		if (keyboard.IsKeyDown(DIK_LEFT) || keyboard.IsKeyDown(DIK_RIGHT)) {
			int keySign = keyboard.IsKeyDown(DIK_LEFT) ? -1 : 1;
			
			//skid start detect
			if (m->IsOnGround() && m->GetVelocity().x * keySign < 0) {
				m->GetSkid() = m->GetVelocity().x / abs(m->GetVelocity().x);
			}

			m->SetMovingState(MovingStates::WALK);
			m->GetAccelerate().x = MARIO_WALK_ACCELERATION * keySign;
			m->GetDrag() = MARIO_WALK_DRAG_FORCE;
			float maxSpeed = MARIO_WALK_SPEED;

			if (keyboard.IsKeyDown(DIK_A)) {
				m->SetMovingState(MovingStates::RUN);
				m->GetAccelerate().x = (m->GetSkid() ? MARIO_SKID_ACCELERATION : MARIO_RUN_ACCELERATION) * keySign;
				m->GetDrag() = MARIO_RUN_DRAG_FORCE;
				maxSpeed = MARIO_RUN_SPEED;
			}

			m->GetVelocity().x += m->GetAccelerate().x * dt;

			if (abs(m->GetVelocity().x) > maxSpeed) {
				m->GetVelocity().x = maxSpeed * keySign;
			}
			m->SetFacing(m->GetVelocity().x < 0 ? -1 : 1);
			
			//skid end detect
			if (m->GetSkid() * m->GetVelocity().x <= 0) {
				m->GetSkid() = 0;
			}
		}
		else {
			if (abs(m->GetVelocity().x) > m->GetDrag() *dt) {
				int sign = m->GetVelocity().x < 0 ? -1 : 1;
				m->GetVelocity().x -= m->GetDrag() * dt * sign;
			}
			else {
				m->GetVelocity().x = 0.0f;
				m->SetMovingState(MovingStates::IDLE);
			}
			m->GetSkid() = 0;
		}

		float maxRun = abs(m->GetVelocity().x) > MARIO_RUN_SPEED * 0.85f;
		if (maxRun && m->GetPowerMeter() < PMETER_MAX + 1)
			m->GetPowerMeter() = max(0.0f, min(m->GetPowerMeter() + PMETER_STEP * dt, PMETER_MAX + 1));
		else if (m->GetPowerMeter() > 0)
			m->GetPowerMeter() = max(0.0f, min(m->GetPowerMeter() - PMETER_STEP * dt, PMETER_MAX));

		//Vertical process
		float jumpForce = MARIO_JUMP_FORCE;
		switch (m->GetJumpingState())
		{
		case JumpingStates::JUMP:
			m->SetCanHighJump(false);

			if (keyboard.IsKeyDown(DIK_S) && m->CanHighJump() && m->GetVelocity().y <= -0.5f * MARIO_JUMP_FORCE &&
				-(maxRun && m->GetPowerMeter() >= PMETER_MAX ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE) <= m->GetVelocity().y) 
			{
				jumpForce = maxRun && m->GetPowerMeter() >= PMETER_MAX ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE;
				m->SetCanHighJump(true);
			}

			if (m->GetVelocity().y > -jumpForce && m->GetVelocity().y < 0 && m->CanHighJump())
			{
				m->GetGravity() = 0;
				m->GetVelocity().y -= MARIO_PUSH_FORCE * dt;
				DebugOut(L"Distance: %f\t%f\n", m->GetDistance().x, m->GetDistance().y);
			}
			else
			{
				m->GetVelocity().y = -1 * jumpForce;
				m->SetJumpingState(JumpingStates::HIGH_JUMP);
				m->GetGravity() = MARIO_GRAVITY;
			}

			break;
		case JumpingStates::HIGH_JUMP:
			if (m->GetVelocity().y > 0) {
				m->SetCanHighJump(false);
				m->SetJumpingState(JumpingStates::FALLING);
			}
			break;
		case JumpingStates::FALLING:
			if (m->IsOnGround()) {
				m->SetJumpingState(JumpingStates::IDLE);
			}
			break;
		}

		m->GetVelocity().y += m->GetGravity() * dt;
		m->GetDistance() = m->GetVelocity() * dt;
	}
}

void MarioPowerUp::Update(vector<shared_ptr<IColliable>>* coObj)
{
	CollisionUpdate(coObj);
	MovementUpdate();
}

void MarioPowerUp::Render()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		string testid = "ani-big-mario-walk";
		AnimationManager::GetInstance()->Get(testid)->GetTransform()->Position = m->GetPosition() - cam;
		AnimationManager::GetInstance()->Get(testid)->Render();
	}
}

void MarioPowerUp::OnKeyDown(int key)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (key == DIK_S && m->IsOnGround() && m->GetJumpingState() == JumpingStates::IDLE) {
			m->GetVelocity().y = -MARIO_JUMP_FORCE*2;
			m->SetJumpingState(JumpingStates::JUMP);
			m->SetOnGround(false);
			m->SetCanHighJump(true);
		}
	}
}

void MarioPowerUp::OnKeyUp(int key)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (key == DIK_S){
			m->CanHighJump() = false;
		}
	}
}
