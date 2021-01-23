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
	collisionCal->Clear();
	SetPowerUp(MarioGame::GetInstance()->GetPlayerData()->Power);
}

void Mario::SetPowerUp(shared_ptr<MarioPower> power, bool effect)
{
	if (power == nullptr) return;
	if (this->power == nullptr) {
		this->power = power;
		return;
	}

	ObjectType cur = this->power->GetMarioType();
	ObjectType next = power->GetMarioType();

	this->power = power;
	MarioGame::GetInstance()->GetPlayerData()->Power = next;

	if (!effect) return;

	if (cur == next) return;
	if (freezeTimer.IsRunning()) return;

	if (cur == MEntityType::SmallMario && next == MEntityType::BigMario) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<GrowUpFX>(Position, this->power->GetMario()));
		freezeTime = 1440;
	}
	else if (cur == MEntityType::BigMario && next == MEntityType::SmallMario) {
		EffectServer::GetInstance()->SpawnEffect(make_shared<ShrinkDownFX>(Position, this->power->GetMario()));
		freezeTime = 1620;
	}
	else {
		EffectServer::GetInstance()->SpawnEffect(make_shared<TransformFX>(Position, this->power->GetMario()));
		freezeTime = 600;
	}

	Visible = false;
	freezeTimer.Restart();
	CGame::GetInstance()->SetTimeScale(0.0f);
}

void Mario::SetPowerUp(ObjectType power)
{
	if (power == MEntityType::SmallMario) {
		this->power = make_shared<Small>(MarioGame::GetInstance()->GetMario());
	} 
	else if (power == MEntityType::BigMario) {
		this->power = make_shared<BigMario>(MarioGame::GetInstance()->GetMario());
	} 
	else if (power == MEntityType::RaccoonMario) {
		this->power = make_shared<RaccoonMario>(MarioGame::GetInstance()->GetMario());
	}
	else if (power == MEntityType::FireMario) {
		this->power = make_shared<FireMario>(MarioGame::GetInstance()->GetMario());
	}
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
	OverlapUpdateOriginal();
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

	if (cameraLimit) {
		CameraRegion* region = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetActiveRegion();
		RectF camBound = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();

		RectF insideBound = region->GetInsideBoundary();
		insideBound.left += camBound.left;
		insideBound.top += camBound.top;
		insideBound.right += camBound.right;
		insideBound.bottom += camBound.bottom;

		RectF targetBound = GetHitBox();
		Vec2 targetPos = GetPosition();
		Vec2 targetSize = Vec2(targetBound.right - targetBound.left, targetBound.bottom - targetBound.top);

		if (targetBound.left < insideBound.left) {
			Velocity.x = region->GetVelocity().x*2;
			movingState = MovingStates::WALK;
		};
		if (targetBound.right > insideBound.right) {
			Velocity.x = region->GetVelocity().x*2;
			movingState = MovingStates::WALK;
		};
		if (targetBound.top < insideBound.top) targetPos.y = insideBound.top;
		if (targetBound.bottom > insideBound.bottom) targetPos.y = insideBound.bottom - targetSize.y;

		SetPosition(targetPos);
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
}

void Mario::Render(D3DCOLOR overlay)
{
	power->Render(overlay);
}

shared_ptr<RayCast> Mario::GetRayCaster()
{
	return raycaster;
}

void Mario::ResetFreeze()
{
	this->freezeTime = 0;
	this->freezeTimer.Stop();
	this->freezeTimer.Reset();
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
			this->SetKickCountDown(300);
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