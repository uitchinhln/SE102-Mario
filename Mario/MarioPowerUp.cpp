#include "MarioPowerUp.h"
#include "Mario.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Koopas.h"

MarioPowerUp::MarioPowerUp(shared_ptr<Mario> mario)
{
	this->mario = mario;
}

void MarioPowerUp::InitResource(bool force)
{
}

RectF MarioPowerUp::GetHitBox()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 pos = m->GetPosition();
		return RectF(pos.x, pos.y, pos.x + sizeFixed.x, pos.y + sizeFixed.y);
	}
	return RectF(0, 0, 0, 0);
}

void MarioPowerUp::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {

		shared_ptr<CollisionCalculator> collisionCal = m->GetCollisionCalc();

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);
	}
}

void MarioPowerUp::StatusUpdate()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {

		shared_ptr<CollisionCalculator> collisionCal = m->GetCollisionCalc();

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->GetLastResults();

		if (coResult.size() == 0)
		{
			//m->GetPosition() += m->GetDistance();
			m->SetOnGround(false);
			if (m->GetDistance().y > 0 && m->GetJumpingState() != JumpingStates::SUPER_JUMP) {
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

		m->SetOnGround(false);
		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (coll->SAABBResult.Direction == Direction::Top && !coll->GameColliableObject->IsGetThrough(*m, coll->SAABBResult.Direction)) {
				m->SetOnGround(true);
				m->SetJumpingState(JumpingStates::IDLE);
			}

			if (MEntityType::IsEnemy(coll->GameColliableObject->GetObjectType())) {
				if (coll->GameColliableObject->GetObjectType() == MEntityType::KoopasCrouch) {
					KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
					shared_ptr<Koopas> koopas = dynamic_pointer_cast<Koopas>(coll->GameColliableObject);

					if (keyboard.IsKeyDown(DIK_A) && !m->GetInhand()) {
						m->SetInhand(koopas);
						koopas->SetHolder(m);
					}
				}

				float damage = coll->GameColliableObject->GetDamageFor(*m, coll->SAABBResult.Direction);
				if (damage > 0) {
					//Die, down level...
					DebugOut(L"Damage from enemy: %f\n", damage);
				}
				else {
					if (!coll->GameColliableObject->IsGetThrough(*m, coll->SAABBResult.Direction) && coll->SAABBResult.Direction == Direction::Top) {
						if (coll->GameColliableObject->GetObjectType() != MEntityType::KoopasCrouch) {
							m->SetOnGround(true);
							m->SetJumpingState(JumpingStates::IDLE);
							MiniJumpDetect(true);
						}
					}
				}
			}
		}
		// No collision occured, proceed normally
	}
}

void MarioPowerUp::MoveUpdate()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		if (keyboard.IsKeyDown(DIK_LEFT) || keyboard.IsKeyDown(DIK_RIGHT)) {
			int keySign = keyboard.IsKeyDown(DIK_LEFT) ? -1 : 1;

			//skid start detect
			if (m->IsOnGround() && m->GetVelocity().x * keySign < 0) {
				m->GetSkid() = (int) (m->GetVelocity().x / abs(m->GetVelocity().x));
			}
			else {
				m->SetSkid(0);
			}

			if (m->GetMovingState() != MovingStates::CROUCH || m->IsOnGround()) {
				m->SetMovingState(MovingStates::WALK);
			}
			m->GetAccelerate().x = MARIO_WALK_ACCELERATION * keySign;
			float maxSpeed = MARIO_WALK_SPEED;

			if (keyboard.IsKeyDown(DIK_A)) {
				if (m->GetMovingState() != MovingStates::CROUCH || m->IsOnGround()) {
					m->SetMovingState(MovingStates::RUN);
				}
				m->GetAccelerate().x = (m->GetSkid() ? MARIO_SKID_ACCELERATION : MARIO_RUN_ACCELERATION) * keySign;
				maxSpeed = MARIO_RUN_SPEED;
			}

			m->GetVelocity().x += m->GetAccelerate().x * dt;

			if (abs(m->GetVelocity().x) > maxSpeed) {
				if (abs(m->GetVelocity().x) - maxSpeed > MARIO_RUN_DRAG_FORCE * dt) {
					int sign = m->GetVelocity().x < 0 ? -1 : 1;
					m->GetVelocity().x -= MARIO_RUN_DRAG_FORCE * dt * sign;
				}
				else {
					m->GetVelocity().x = maxSpeed * keySign;
				}

			}

			m->SetFacing(m->IsOnGround() ? (m->GetVelocity().x < 0 ? -1 : 1) : keySign);

			//skid end detect
			if (m->GetSkid() * m->GetVelocity().x <= 0) {
				m->GetSkid() = 0;
			}
		}
		else if (m->GetMovingState() != MovingStates::CROUCH) {
			if (abs(m->GetVelocity().x) > m->GetDrag() * dt) {
				int sign = m->GetVelocity().x < 0 ? -1 : 1;
				m->GetVelocity().x -= m->GetDrag() * dt * sign;
			}
			else {
				m->GetVelocity().x = 0.0f;
				m->SetMovingState(MovingStates::IDLE);
			}
			m->GetSkid() = 0;
		}

		if (m->IsOnGround()) {
			m->GetDrag() = m->GetMovingState() == MovingStates::WALK ? MARIO_WALK_DRAG_FORCE : MARIO_RUN_DRAG_FORCE;
		}
		else {
			m->GetDrag() = 0;
		}
	}
}

void MarioPowerUp::PowerMeterUpdate()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		float maxRun = abs(m->GetVelocity().x) > MARIO_RUN_SPEED * 0.85f;

		if (maxRun && m->IsOnGround())
			m->GetPowerMeter() = max(0.0f, min(m->GetPowerMeter() + PMETER_UP_STEP * dt, PMETER_MAX + 1));
		else if (m->GetPowerMeter() > 0)
			m->GetPowerMeter() = max(0.0f, min(m->GetPowerMeter() - PMETER_DOWN_STEP * dt, PMETER_MAX));
	}
}

void MarioPowerUp::MiniJumpDetect(bool forceX)
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		if (keyboard.IsKeyDown(DIK_X) || forceX) {
			if (m->IsOnGround() && m->GetJumpingState() == JumpingStates::IDLE) {
				//m->GetVelocity().y = -MARIO_JUMP_FORCE;
				m->GetVelocity().y = -0.1f;
				m->SetJumpingState(JumpingStates::JUMP);
				m->SetOnGround(false);
				m->SetCanHighJump(false);
			}
		}
	}
}

void MarioPowerUp::JumpUpdate()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {

		float maxRun = abs(m->GetVelocity().x) > MARIO_RUN_SPEED * 0.85f;

		float jumpForce = MARIO_JUMP_FORCE;

		switch (m->GetJumpingState())
		{
		case JumpingStates::SUPER_JUMP:
			if (keyboard.IsKeyDown(DIK_S))
				jumpForce = MARIO_SUPER_JUMP_FORCE;

			if (m->GetVelocity().y > -jumpForce && m->GetVelocity().y < 0 && m->CanHighJump())
			{
				m->GetGravity() = 0;
				m->GetVelocity().y -= MARIO_PUSH_FORCE * dt;
			}
			else
			{
				m->GetGravity() = MARIO_GRAVITY;
				m->SetCanHighJump(false);
			}
			break;
		case JumpingStates::JUMP:
			if (keyboard.IsKeyDown(DIK_S) && m->CanHighJump() && -MARIO_HIGH_JUMP_FORCE <= m->GetVelocity().y && m->GetVelocity().y <= -0.5f * MARIO_JUMP_FORCE)
			{
				jumpForce = MARIO_HIGH_JUMP_FORCE;
			}

			if (m->GetVelocity().y > -jumpForce && m->GetVelocity().y < 0)
			{
				m->GetGravity() = 0;
				m->GetVelocity().y -= MARIO_PUSH_FORCE * dt;
			}
			else
			{
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
			m->GetGravity() = MARIO_GRAVITY;
			break;
		}
	}
}

void MarioPowerUp::CrouchUpdate()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
		if (keyboard.IsKeyDown(DIK_DOWN) && m->GetJumpingState() == JumpingStates::IDLE
			&& !keyboard.IsKeyDown(DIK_LEFT) && !keyboard.IsKeyDown(DIK_RIGHT)
			&& !m->GetInhand()) {

			m->SetMovingState(MovingStates::CROUCH);

			if (abs(m->GetVelocity().x) > MARIO_CROUCH_DRAG_FORCE * dt) {
				int sign = m->GetVelocity().x < 0 ? -1 : 1;
				m->GetVelocity().x -= MARIO_CROUCH_DRAG_FORCE * dt * sign;
			}
			else {
				m->GetVelocity().x = 0.0f;
			}
		}

		Vec2 pos = m->GetPosition();
		Vec2 fixedPos = Vec2(pos.x, pos.y + sizeFixed.y);

		if (m->GetMovingState() == MovingStates::CROUCH) {
			sizeFixed = Vec2(size.x, 45);
		}
		else {
			sizeFixed = size;
		}
		m->SetPosition(Vec2(fixedPos.x, fixedPos.y - sizeFixed.y));
	}
}

void MarioPowerUp::Update()
{
	MoveUpdate();
	PowerMeterUpdate();
	MiniJumpDetect();
	JumpUpdate();
	CrouchUpdate();

	if (shared_ptr<Mario> m = mario.lock()) {
		DWORD dt = CGame::Time().ElapsedGameTime;

		m->GetVelocity().y += m->GetGravity() * (float)dt;
		m->GetDistance() = m->GetVelocity() * (float)dt;
	}
}

void MarioPowerUp::JumpAnimation()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		bool apply = false;
		switch (m->GetJumpingState())
		{
		case JumpingStates::SUPER_JUMP:
			selectedAnimation = animations["Fly"];
			apply = true;
			break;
		case JumpingStates::JUMP:
			selectedAnimation = animations["Jump"];
			apply = true;
			break;
		case JumpingStates::HIGH_JUMP:
			selectedAnimation = animations["Jump"];
			apply = true;
			break;
		case JumpingStates::FALLING:
		case JumpingStates::FLOATING:
			selectedAnimation = animations["Fall"];
			apply = true;
			break;
		}

		if (m->GetInhand() && apply) {
			selectedAnimation = animations["HoldFall"];
		}
	}
}

void MarioPowerUp::MoveAnimation()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (m->GetJumpingState() != JumpingStates::IDLE && !m->IsOnGround()) return;

		if (m->GetSkid()) {
			selectedAnimation = animations["Skid"];

			if (m->GetInhand()) {
				selectedAnimation = animations["Hold"];
			}
		}
		else if (abs(m->GetVelocity().x) <= m->GetDrag() * 7) {
			selectedAnimation = animations["Idle"];

			if (m->GetInhand()) {
				selectedAnimation = animations["HoldIdle"];
			}
		}
		else {
			switch (m->GetMovingState())
			{
			case MovingStates::RUN:
				selectedAnimation = animations["Run"];
				if (m->GetPowerMeter() >= PMETER_MAX) {
					selectedAnimation = animations["HighSpeed"];
				}
				break;
			case MovingStates::WALK:
				selectedAnimation = animations["Walk"];
				break;
			}

			if (m->GetInhand()) {
				selectedAnimation = animations["Hold"];
			}
		}
	}
}

void MarioPowerUp::Render()
{
	this->InitResource();

	if (shared_ptr<Mario> m = mario.lock()) {
		JumpAnimation();
		MoveAnimation();

		if (m->GetMovingState() == MovingStates::CROUCH) {
			selectedAnimation = animations["Crouch"];
		}

		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		selectedAnimation->SetPlayScale(max(0.4f, min(abs(m->GetVelocity().x) / MARIO_WALK_SPEED, 4)) * 1.5f);
		selectedAnimation->GetTransform()->Scale = Vec2((float)m->GetFacing(), 1);
		selectedAnimation->GetTransform()->Position = m->GetPosition() - cam;
		selectedAnimation->Render();
	}
}

void MarioPowerUp::OnKeyDown(int key)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (key == DIK_S) {
			if (m->IsOnGround() && m->GetJumpingState() == JumpingStates::IDLE) {
				MiniJumpDetect(true);
				m->SetCanHighJump(true);

				if (m->GetPowerMeter() >= PMETER_MAX) {
					m->SetJumpingState(JumpingStates::SUPER_JUMP);
				}
			}
			else {
				MiniJumpDetect(true);
			}
		}
	}
}

void MarioPowerUp::OnKeyUp(int key)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (key == DIK_S) {
			m->SetCanHighJump(false);
		}

		if (key == DIK_DOWN) {
			m->SetMovingState(MovingStates::IDLE);
		}
	}
}

void MarioPowerUp::Init(TiXmlElement* data)
{
	TiXmlElement* anis = data->FirstChildElement("AnimationSet");
	for (TiXmlElement* node = anis->FirstChildElement(); node != NULL; node = node->NextSiblingElement()) {
		this->animations[node->Value()] = AnimationManager::GetInstance()->Get(node->Attribute("animationId"))->Clone();
	}
}

shared_ptr<Mario> MarioPowerUp::GetMario()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		return m;
	}
	return nullptr;
}

MarioPowerUp::~MarioPowerUp()
{
	//DebugOut(L"Huy mario power\n");
}
