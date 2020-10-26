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
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {

		m->GetVelocity().y += m->GetGravity() * dt;
		m->GetDistance() = m->GetVelocity() * dt;

		shared_ptr<CollisionCalculator> collisionCal = m->GetCollisionCalc();

		vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

		// No collision occured, proceed normally
		if (coResult.size() == 0)
		{
			m->GetPosition() += m->GetDistance();
			m->SetOnGround(false);
			if (m->GetVelocity().y > 0) {
				m->SetJumpingState(JumpingStates::FALLING);
			}
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
			if (coll->SAABBResult.Direction == Direction::Top && !coll->GameColliableObject->IsGetThrough(*m, coll->SAABBResult.Direction)) {
				m->SetOnGround(true);
				m->SetJumpingState(JumpingStates::IDLE);
				break;
			}
			else {
				m->SetOnGround(false);
			}
		}
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
	}
}

void MarioPowerUp::JumpUpdate()
{
	KeyboardProcessor keyboard = CGame::GetInstance()->GetKeyBoard();
	DWORD dt = CGame::Time().ElapsedGameTime;

	if (shared_ptr<Mario> m = mario.lock()) {
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
			if (keyboard.IsKeyDown(DIK_S) && m->CanHighJump() &&
				-(maxRun && m->GetPowerMeter() >= PMETER_MAX ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE) <= m->GetVelocity().y &&
				m->GetVelocity().y <= -0.5f * MARIO_JUMP_FORCE)
			{
				jumpForce = maxRun && m->GetPowerMeter() >= PMETER_MAX ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE;
			}

			if (m->GetVelocity().y > -jumpForce && m->GetVelocity().y < 0 && m->CanHighJump())
			{
				m->GetGravity() = 0;
				m->GetVelocity().y -= MARIO_PUSH_FORCE * dt;
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
		}
	}
}

void MarioPowerUp::Update(vector<shared_ptr<IColliable>>* coObj)
{
	MoveUpdate();
	JumpUpdate();
	CollisionUpdate(coObj);
}

void MarioPowerUp::Render()
{
	if (shared_ptr<Mario> m = mario.lock()) {		
		Animation ani = animations["Idle"];

		if (m->GetJumpingState() != JumpingStates::IDLE) {
			if (m->GetPowerMeter() >= 1) {
				ani = animations["Fly"];
			}
			else {
				switch (m->GetJumpingState())
				{
				case JumpingStates::JUMP:
					ani = animations["Jump"];
					break;
				case JumpingStates::HIGH_JUMP:
					ani = animations["Jump"];
					break;
				case JumpingStates::FALLING:
					ani = animations["Fall"];
					break;
				}
			}
		}
		else {
			if (m->GetSkid()) {
				ani = animations["Skid"];
			}
			else if (abs(m->GetVelocity().x) <= m->GetDrag()*7) {
				ani = animations["Idle"];
			}
			else {
				switch (m->GetMovingState())
				{
				case MovingStates::RUN:
					ani = animations["Run"];
					break;
				case MovingStates::WALK:
					ani = animations["Walk"];
					break;
				}
			}
		}

		Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

		ani->GetTransform()->Scale = Vec2(m->GetFacing(), 1);
		ani->GetTransform()->Position = m->GetPosition() - cam;
		ani->Render();
	}
}

void MarioPowerUp::OnKeyDown(int key)
{
	if (shared_ptr<Mario> m = mario.lock()) {
		if (key == DIK_S && m->IsOnGround() && m->GetJumpingState() == JumpingStates::IDLE) {
			m->GetVelocity().y = -MARIO_JUMP_FORCE;
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
			m->SetCanHighJump(false);
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
