#include "Koopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"

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

void Koopas::SetPower(shared_ptr<DefaultKoopas> power)
{
	if (!power) return;
	this->power = power;
}

void Koopas::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	power->CollisionUpdate(coObj);
}

void Koopas::StatusUpdate()
{
	power->StatusUpdate();
}

void Koopas::Update()
{
	power->Update();
}

void Koopas::FinalUpdate()
{
	power->FinalUpdate();
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
	return state == KoopasLiveStates::DIE;
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
