#include "Koopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "CrouchKoopas.h"
#include "Mario.h"

Koopas::Koopas()
{
	//this->animations["Move"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-move")->Clone();
	//this->animations["Crouch"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-crouch")->Clone();
	//this->animations["RunningShell"] = AnimationManager::GetInstance()->Get("ani-green-koopa-troopa-shell-run")->Clone();

	this->SetPosition(Vec2(0, 0));
}

Stopwatch& Koopas::GetDestroyTimer()
{
	return destroyTimer;
}

KoopasLiveStates& Koopas::GetLiveState()
{
	return state;
}

weak_ptr<Mario> Koopas::GetHolder()
{
	return this->koopas;
}

void Koopas::SetHolder(shared_ptr<Mario> holder)
{
	if (state == KoopasLiveStates::DIE) {
		holder->ClearInhand();
		return;
	}
	this->koopas = holder;
}

void Koopas::ClearHolder()
{
	this->koopas.reset();
}

void Koopas::SetPower(shared_ptr<DefaultKoopas> power)
{
	if (!power) return;
	this->power = power;
}

void Koopas::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	shared_ptr<DefaultKoopas> p = power;
	p->CollisionUpdate(coObj);
}

void Koopas::StatusUpdate()
{
	shared_ptr<DefaultKoopas> p = power;
	p->StatusUpdate();
}

void Koopas::Update()
{
	shared_ptr<DefaultKoopas> p = power;
	p->Update();
}

void Koopas::FinalUpdate()
{
	shared_ptr<DefaultKoopas> p = power;
	p->FinalUpdate();
	collisionCal->Clear();
}

void Koopas::Render()
{
	power->Render();
}

ObjectType Koopas::GetObjectType()
{
	return power->GetObjectType();
}

RectF Koopas::GetHitBox()
{
	return power->GetHitBox();
}

bool Koopas::IsGetThrough(IColliable& object, Direction direction)
{
	//bool koopasImp = object.GetObjectType() == MEntityType::KoopasImposter;
	bool notKoopas = object.GetObjectType() != MEntityType::Koopas;
	bool notKoopasCrouch = object.GetObjectType() != MEntityType::KoopasCrouch;
	return state == KoopasLiveStates::DIE || ((notKoopas || notKoopasCrouch) && MEntityType::IsEnemy(object.GetObjectType()));
}

float Koopas::GetDamageFor(IColliable& object, Direction direction)
{
	return power->GetDamageFor(object, direction);
}

shared_ptr<Koopas> Koopas::CreateKoopas(Vec2 fixedPos)
{
	shared_ptr<Koopas> koopas = make_shared<Koopas>();
	koopas->SetPower(make_shared<DefaultKoopas>(koopas));
	koopas->SetCollisionCalculator(make_shared<CollisionCalculator>(koopas));
	float height = abs((koopas->GetHitBox().bottom - koopas->GetHitBox().top));
	koopas->SetPosition(Vec2(fixedPos.x, fixedPos.y - height));
	return koopas;
}

Koopas::~Koopas()
{
	//DebugOut(L"Huy koopas\n");
}
