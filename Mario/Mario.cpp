#include "Mario.h"
#include "Game.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "EffectServer.h"
#include "Events.h"
#include "GameEvent.h"
#include "Small.h"
#include "BigMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"
#include "Koopas.h"
#include "Goomba.h"
#include "CrouchKoopas.h"
#include "ScoreFX.h"
#include "MarioDeathFX.h"
#include "GrowUpFX.h"
#include "ShrinkDownFX.h"
#include "TransformFX.h"
#include "MarioGame.h"

void Mario::OnKeyUp(int key)
{
	//if (IsControllerLocked()) return;
	power->OnKeyUp(key);
	switch (key)
	{
	case DIK_A:
		DropShell();
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
	//if (IsControllerLocked()) return;
	shared_ptr<MarioPower> p = power;
	p->OnKeyDown(key);
	
	// Change mario power
	Vec2 fixPos = Vec2(GetHitBox().left, GetHitBox().bottom);
	shared_ptr<Mario> thiss = MarioGame::GetInstance()->GetMario();

	switch (key)
	{
	case DIK_1:
		SetPowerUp(make_shared<Small>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_2:
		SetPowerUp(make_shared<BigMario>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_3:
		SetPowerUp(make_shared<FireMario>(thiss));
		Position.x = fixPos.x;
		Position.y = fixPos.y - (GetHitBox().bottom - GetHitBox().top);
		break;
	case DIK_4:
		SetPowerUp(make_shared<RaccoonMario>(thiss));
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
	case DIK_R:
		Position.x = 6742;
		Position.y = 370;
		SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetBoundingEdge(Direction::Top, 0);
		break;
	case DIK_E:
		Position.x = 7900;
		Position.y = 1136;
		break;
	}

}

void Mario::OnDamaged(float damage)
{
	if (invulnerable > 0) return;
	shared_ptr<MarioPower> p = power;
	p->OnDamaged(damage);
}

void Mario::OnDeath()
{
	collidable = false;
	if (invulnerable > 0) return;
	SceneManager::GetInstance()->GetActiveScene()->GetCamera()->SetFreeze(true);
	EffectServer::GetInstance()->SpawnEffect(make_shared<MarioDeathFX>(Position));
	this->Visible = false;
	this->SetLockController(true);
	this->invulnerable = 9999999;

	__raise (*GameEvent::GetInstance()).PlaySceneEndEvent(__FILE__, SceneResult::LOSE, CardType::EMPTY);
}

void Mario::OnPowerUp(ObjectType powerType)
{
	if (powerType == MEntityType::GreenMushroom) {
		__raise (*GameEvent::GetInstance()).PlayerLifeChangeEvent(__FILE__, Position);
	}
	else {
		shared_ptr<MarioPower> p = power;
		p->OnPowerUp(powerType);
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
	this->freezeTimer.Stop();
}

void Mario::Reset()
{
	GameObject::Reset();
	controllable = true;
	sliding = false;
	onGround = true;
	invulnerable = 0;
	freezeTime = 0;
	freezeTimer.Stop();
	freezeTimer.Reset();
	drag = 0;
	skid = 0;
	kickCountDown = 0;
	jumpBeginPos = 0;
	powerMeter = 0;
	accelerate = VECTOR_0;
	movingState = MovingStates::IDLE;
	jumpingState = JumpingStates::IDLE;
	warpState = WarpStates::NONE;
	inhand.reset();
	raycaster->Clear();
	Gravity = 0.00093f;
	renderOrder = 1001;
}

void Mario::SetPowerUp(shared_ptr<MarioPower> power)
{
	if (power == nullptr) return;
	if (this->power == nullptr) {
		this->power = power;
		return;
	}

	ObjectType cur = this->power->GetMarioType();
	ObjectType next = power->GetMarioType();

	shared_ptr<Mario> thiss = MarioGame::GetInstance()->GetMario();

	this->power = power;
	MarioGame::GetInstance()->GetPlayerData()->Power = next;

	if (cur == next) return;
	if (freezeTimer.IsRunning()) return;

	if (cur == MEntityType::SmallMario && next == MEntityType::BigMario) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<GrowUpFX>(Position, thiss));
		freezeTime = 1440;
	}
	else if (cur == MEntityType::BigMario && next == MEntityType::SmallMario) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<ShrinkDownFX>(Position, thiss));
		freezeTime = 1620;
	}
	else {
		EffectServer::GetInstance()->SpawnEffect(make_shared<TransformFX>(Position, thiss));
		freezeTime = 600;
	}

	Visible = false;
	freezeTimer.Restart();
	CGame::GetInstance()->SetTimeScale(0.0f);
}

void Mario::InitResource()
{
}

void Mario::StatusUpdate()
{
	shared_ptr<MarioPower> p = power;
	p->StatusUpdate();
}

void Mario::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	shared_ptr<MarioPower> p = power;
	p->CollisionUpdate(coObj);
	raycaster->SetInput(coObj);
	//auto start = std::chrono::high_resolution_clock::now();
	OverlapUpdateOriginal();
	//auto finish = std::chrono::high_resolution_clock::now();
	//DebugOut(L"Mario raycast: %d\n", std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());
}

void Mario::PositionUpdate()
{
	GameObject::PositionUpdate();

	RectF hitbox = GetHitBox();
	RectF limit = MovingBound;

	limit.left += hitbox.right - hitbox.left;
	limit.top += hitbox.bottom - hitbox.top;
	limit.right -= hitbox.right - hitbox.left;
	limit.bottom -= hitbox.bottom - hitbox.top;


	if (!collisionCal->AABB(limit, hitbox)) {
		Position.x = max(limit.left - (hitbox.right - hitbox.left), min(Position.x, limit.right));
		Position.y = min(Position.y, limit.bottom);
		Velocity.x = 0;
	}
}

void Mario::FinalUpdate()
{
	GameObject::FinalUpdate();
	collisionCal->Clear();
}

void Mario::Update()
{
	if (freezeTimer.IsRunning() && freezeTimer.Elapsed() > freezeTime) {
		freezeTimer.Stop();

		CGame::GetInstance()->SetTimeScale(1.0f);
		Visible = true;
	}
	invulnerable -= CGame::Time().ElapsedGameTime;
	//collidable = invulnerable <= 0;

	if (!controllable) return;
	shared_ptr<MarioPower> p = power;
	p->Update();

	switch (warpState)
	{
	case WarpStates::NONE:
		renderOrder = 1001;
		break;
	case WarpStates::VERTICAL:
		renderOrder = 124;
		break;
	case WarpStates::HORIZONTAL:
		renderOrder = 499;
		break;
	}
}

void Mario::Render()
{
	power->Render();
}

shared_ptr<RayCast> Mario::GetRayCaster()
{
	return raycaster;
}

bool Mario::IsControllerLocked()
{
	return !controllable || sliding || warpState != WarpStates::NONE;
}

void Mario::SetLockController(bool value)
{
	this->controllable = !value;
}

void Mario::DropShell()
{
	if (IsControllerLocked()) return;
	if (shared_ptr<GameObject> obj = inhand.lock()) {
		if (obj->GetObjectType() == MEntityType::KoopasPassenger) {
			shared_ptr<Koopas> koopas = dynamic_pointer_cast<Koopas>(obj);

			Vec2 size = Vec2(
				koopas->GetHitBox().right - koopas->GetHitBox().left,
				koopas->GetHitBox().bottom - koopas->GetHitBox().top
			);

			if (GetFacing() > 0) {
				koopas->SetPosition(Vec2(
					GetHitBox().right + 2,
					koopas->GetHitBox().top));
			}
			else {
				koopas->SetPosition(Vec2(
					GetHitBox().left - size.x - 2,
					koopas->GetHitBox().top));
			}
			Velocity.x += GetFacing() * 0.1f;

			koopas->ClearHolder();
			this->ClearInhand();
		}
	}
	else {
		inhand.reset();
	}
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
	//__hook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	//__hook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}

void Mario::UnHookEvent()
{
	//__unhook(&Events::KeyDownEvent, Events::GetInstance(), &Mario::OnKeyDown);
	//__unhook(&Events::KeyUpEvent, Events::GetInstance(), &Mario::OnKeyUp);
}