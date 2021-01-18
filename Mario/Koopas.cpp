#include "Koopas.h"
#include "MEntityType.h"
#include "AnimationManager.h"
#include "CrouchKoopas.h"
#include "Mario.h"
#include "SceneManager.h"

Koopas::Koopas()
{
	this->SetPosition(Vec2(0, 0));
}

KoopasLifeStates& Koopas::GetLiveState()
{
	return state;
}

weak_ptr<Mario> Koopas::GetHolder()
{
	return this->koopas;
}

void Koopas::SetHolder(shared_ptr<Mario> holder)
{
	if (state == KoopasLifeStates::DIE) {
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

void Koopas::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	shared_ptr<DefaultKoopas> p = power;
	p->CollisionUpdate(coObj);
}

void Koopas::PositionUpdate()
{
	shared_ptr<DefaultKoopas> p = power;
	p->PositionUpdate();
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

bool Koopas::IsGetThrough(GameObject& object, Direction direction)
{
	if (direction == Direction::None) return true;
	if (MEntityType::IsPowerUpItem(object.GetObjectType())) return true;
	//bool koopasImp = object.GetObjectType() == MEntityType::KoopasImposter;
	bool notKoopas = object.GetObjectType() != MEntityType::Koopas;
	bool notKoopasCrouch = object.GetObjectType() != MEntityType::KoopasCrouch;
	return state == KoopasLifeStates::DIE || ((notKoopas && notKoopasCrouch) && MEntityType::IsEnemy(object.GetObjectType()));
}

float Koopas::GetDamageFor(GameObject& object, Direction direction)
{
	return power->GetDamageFor(object, direction);
}

void Koopas::OnLostCamera()
{
	if (state == KoopasLifeStates::DIE) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}
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
	DebugOut(L"Huy koopas\n");
}
