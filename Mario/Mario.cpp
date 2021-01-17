#include "Mario.h"
#include "Game.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "Events.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"
#include "Koopas.h"
#include "Goomba.h"
#include "CrouchKoopas.h"

void Mario::OnKeyUp(int key)
{
	if (!controllable) return;
	powerUp->OnKeyUp(key);
	switch (key)
	{
	case DIK_A:
		if (shared_ptr<GameObject> obj = inhand.lock()) {
			if (obj->GetObjectType() == MEntityType::KoopasPassenger) {
				shared_ptr<Koopas> koopas = dynamic_pointer_cast<Koopas>(obj);

				Vec2 size = Vec2(
					koopas->GetHitBox().right - koopas->GetHitBox().left,
					koopas->GetHitBox().bottom - koopas->GetHitBox().top
				);

				if (GetFacing() > 0) {
					obj->SetPosition(Vec2(
						GetHitBox().right + 2,
						obj->GetHitBox().top));
				}
				else {
					obj->SetPosition(Vec2(
						GetHitBox().left - size.x - 2,
						obj->GetHitBox().top));
				}
				Velocity.x += GetFacing();

				koopas->ClearHolder();
				this->ClearInhand();
			}
		}
		else {
			inhand.reset();
		}
		break;
	case DIK_Q:
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(Goomba::CreateGoomba(Position - Vec2(0, 230)));
		break;
	case DIK_D:
		shared_ptr<Koopas> kp = Koopas::CreateKoopas(Position - Vec2(0, 230));
		kp->SetPower(make_shared<CrouchKoopas>(kp));
		SceneManager::GetInstance()->GetActiveScene()->SpawnEntity(kp);
		break;
	}
}

void Mario::OnKeyDown(int key)
{
	if (!controllable) return;
	shared_ptr<MarioPowerUp> p = powerUp;
	p->OnKeyDown(key);
	
	// Change mario power
	Vec2 fixPos = Vec2(GetHitBox().left, GetHitBox().bottom);

	switch (key)
	{
	case DIK_1:
		SetPowerUp(make_shared<Small>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_2:
		SetPowerUp(make_shared<BigMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_3:
		SetPowerUp(make_shared<FireMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_4:
		SetPowerUp(make_shared<RaccoonMario>(shared_from_this()));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_P:
		DebugOut(L"Position: x = %f\ty = %f\n", Position.x, Position.y);
		break;
	case DIK_T:
		Position.x = 5711;
		Position.y = 1150;
		break;
	}

}

Mario::Mario() : GameObject()
{
	this->Distance = Vec2(0, 0);
	this->Position = Vec2(100, 1200);
	this->Velocity = Vec2(0, 0);
	this->accelerate = Vec2(0, 0);
	this->Gravity = 0.00093f;
	this->renderOrder = 1001;
	this->raycaster = make_shared<RayCast>();
	HookEvent();
}

void Mario::SetPowerUp(shared_ptr<MarioPowerUp> powerUp)
{
	this->powerUp = powerUp;
}

void Mario::InitResource()
{
}

void Mario::OverlapUpdate()
{
	vector<shared_ptr<CollisionResult>> overlapsEvents = collisionCal->GetOverlaps();
	if (overlapsEvents.size() > 0) {
		overlapsEvents.erase(remove_if(overlapsEvents.begin(), overlapsEvents.end(), [](const shared_ptr<CollisionResult>& evt) {
			return !MEntityType::IsTile(evt->Object->GetObjectType());
			}), overlapsEvents.end());
	}

	if (overlapsEvents.size() > 0) {
		DWORD marioId = this->GetID();
		RectF hitbox = GetHitBox();
		float marioWidth = hitbox.right - hitbox.left;
		Vec2 headPoint = Vec2(hitbox.left, hitbox.top + (hitbox.bottom - hitbox.top) / 4);
		Vec2 legPoint = Vec2(hitbox.left, hitbox.bottom - (hitbox.bottom - hitbox.top) / 4);
		ObjectList headResult;
		ObjectList legResult;
		vector<shared_ptr<Vec2>> headBoxes;
		vector<shared_ptr<Vec2>> legBoxes;

		raycaster->Shoot(headPoint, Direction::Right, 960, headResult);
		raycaster->Shoot(legPoint, Direction::Right, 960, legResult);

		raycaster->MergeBox(headResult, Direction::Right, marioWidth - 0.0001, headBoxes);
		raycaster->MergeBox(legResult, Direction::Right, marioWidth - 0.0001, legBoxes);

		Vec2 headBox = headBoxes.empty() ? Vec2(99999, -99999) : *headBoxes[0];
		Vec2 legsBox = legBoxes.empty() ? Vec2(99999, -99999) : *legBoxes[0];

		//Debug
		//RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
		//testbox.push_back(RectF(headBox.x - cam.left, headPoint.y - 10 - cam.top, abs(headBox.y - headBox.x), 20));
		//testbox.push_back(RectF(legsBox.x - cam.left, legPoint.y - 10 - cam.top, abs(legsBox.y - legsBox.x), 20));

		Vec2 overBox = legsBox, 
			barrierBox = headBox;
		if (headBox.x <= legsBox.x) {
			overBox = headBox;
			barrierBox = legsBox;
		}

		if (hitbox.left + marioWidth < overBox.x) {
			headPoint.x = headBox.x - 0.1;
			legPoint.x = legsBox.x - 0.1;
			raycaster->Shoot(headPoint, Direction::Left, marioWidth, headResult);
			raycaster->Shoot(legPoint, Direction::Left, marioWidth, legResult);
			if (headResult.size() > 0 || legResult.size() > 0) {
				if (barrierBox.x - overBox.y >= marioWidth) {
					Position.x += 0.7 * CGame::Time().ElapsedGameTime;
				}
				else {
					Position.x -= 0.7 * CGame::Time().ElapsedGameTime;
				}
			}
			else {
				Position.x -= 0.7 * CGame::Time().ElapsedGameTime;
			}
		}
		else {
			if (barrierBox.x - overBox.y >= marioWidth) {
				Position.x += 0.7 * CGame::Time().ElapsedGameTime;
			}
			else {
				Position.x -= 0.7 * CGame::Time().ElapsedGameTime;
			}
		}
	}
}

void Mario::StatusUpdate()
{
	shared_ptr<MarioPowerUp> p = powerUp;
	p->StatusUpdate();
}

void Mario::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	shared_ptr<MarioPowerUp> p = powerUp;
	p->CollisionUpdate(coObj);
	raycaster->SetInput(coObj);
	OverlapUpdate();
}

void Mario::PositionUpdate()
{
	GameObject::PositionUpdate();

	RectF hitbox = GetHitBox();

	Vec2 size(hitbox.right - hitbox.left, hitbox.bottom - hitbox.top);

	//fixed position
	Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
	if (GetPosition().x < 0.3 || GetPosition().x > mapBound.x + 22 || GetPosition().y > mapBound.y - size.y) {
		GetPosition().x = max(0.3f, min(GetPosition().x, mapBound.x + 22));
		GetPosition().y = min(GetPosition().y, mapBound.y - size.y);
		GetVelocity().x = 0;
	}
}

void Mario::FinalUpdate()
{
	GameObject::FinalUpdate();
	collisionCal->Clear();
}

void Mario::Update()
{
	if (!controllable) return;
	shared_ptr<MarioPowerUp> p = powerUp;
	p->Update();
}

void Mario::Render()
{
	powerUp->Render();
}

shared_ptr<RayCast> Mario::GetRayCaster()
{
	return raycaster;
}

bool Mario::IsLockController()
{
	return !controllable;
}

void Mario::SetLockController(bool value)
{
	this->controllable = !value;
}

void Mario::ClearInhand()
{
	this->inhand.reset();
}

void Mario::SetInhand(shared_ptr<GameObject> object)
{
	this->inhand = object;
}

shared_ptr<GameObject> Mario::GetInhand()
{
	if (shared_ptr<GameObject> o = inhand.lock()) {
		return o;
	}
	return nullptr;
}

float& Mario::GetDrag()
{
	return this->drag;
}

void Mario::SetDrag(float drag)
{
	this->drag = drag;
}

int& Mario::GetSkid()
{
	return this->skid;
}

void Mario::SetSkid(int skid)
{
	this->skid = skid;
}

float& Mario::GetJumpBeginPosition()
{
	return this->jumpBeginPos;
}

void Mario::SetJumpBeginPosition(float value)
{
	this->jumpBeginPos = value;
}

float& Mario::GetPowerMeter()
{
	return this->powerMeter;
}

void Mario::SetPowerMeter(float value)
{
	this->powerMeter = value;
}

bool Mario::IsOnGround()
{
	return onGround;
}

void Mario::SetOnGround(bool value)
{
	onGround = value;
}

Vec2& Mario::GetDistance()
{
	return Distance;
}

Vec2& Mario::GetAccelerate()
{
	return accelerate;
}

RectF Mario::GetHitBox()
{
	return powerUp->GetHitBox();
}

MovingStates& Mario::GetMovingState()
{
	return this->movingState;
}

void Mario::SetMovingState(MovingStates state)
{
	this->movingState = state;
}

JumpingStates& Mario::GetJumpingState()
{
	return this->jumpingState;
}

void Mario::SetJumpingState(JumpingStates state)
{
	this->jumpingState = state;
}

ObjectType Mario::GetObjectType()
{
	return powerUp->GetMarioType();
}

bool Mario::IsGetThrough(GameObject& object, Direction direction)
{
	if (direction == Direction::None) return true;
	bool fireballAllow = object.GetObjectType() == MEntityType::MarioFireBall;
	bool tailAllow = object.GetObjectType() == MEntityType::MarioTailed;
	bool enemyAllow = MEntityType::IsEnemy(object.GetObjectType());

	return fireballAllow || tailAllow || enemyAllow;
}

float Mario::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}

void Mario::HookEvent()
{
	__hook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	__hook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}

void Mario::UnHookEvent()
{
	__unhook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	__unhook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}