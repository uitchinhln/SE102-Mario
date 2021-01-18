#include "RedGoomba.h"
#include "Game.h"
#include "MEntityType.h"
#include "JumpingRedGoomba.h"

RedGoomba::RedGoomba()
{
	this->Position = VECTOR_0;
}

RedGoombaState& RedGoomba::GetState()
{
	return state;
}

Vec2& RedGoomba::GetSize()
{
	return this->size;
}

void RedGoomba::SetObjectState(shared_ptr<DefaultRedGoomba> objectState)
{
	this->objectState = objectState;
}

void RedGoomba::InitResource()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->InitResource();
}

void RedGoomba::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->CollisionUpdate(coObj);
}

void RedGoomba::PositionUpdate()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->PositionUpdate();
}

void RedGoomba::StatusUpdate()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->StatusUpdate();
}

void RedGoomba::Update()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->Update();
}

void RedGoomba::FinalUpdate()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->FinalUpdate();
}

void RedGoomba::Render()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	state->Render();
}

ObjectType RedGoomba::GetObjectType()
{
	shared_ptr<DefaultRedGoomba> state = objectState;
	return state->GetObjectType();
}

RectF RedGoomba::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool RedGoomba::IsGetThrough(GameObject& object, Direction direction)
{
	if (direction == Direction::None) return true;
	//bool koopasImp = object.GetObjectType() == MEntityType::KoopasImposter;
	bool notGoomba = object.GetObjectType() != MEntityType::RedGoomba;
	return state != RedGoombaState::WALK || ((notGoomba) && MEntityType::IsEnemy(object.GetObjectType()));
}

float RedGoomba::GetDamageFor(GameObject& object, Direction direction)
{
	if (state == RedGoombaState::WALK && MEntityType::IsMario(object.GetObjectType()) && direction != Direction::Top) {
		return 1.0f;
	}
	if (state == RedGoombaState::WALK && MEntityType::IsMarioWeapon(object.GetObjectType())) {
		return 1.0f;
	}
	return 0.0f;
}

shared_ptr<RedGoomba> RedGoomba::CreateRedGoomba(Vec2 fixedPos)
{
	shared_ptr<RedGoomba> goomba = make_shared<RedGoomba>();
	goomba->SetCollisionCalculator(make_shared<CollisionCalculator>(goomba));
	float height = abs((goomba->GetHitBox().bottom - goomba->GetHitBox().top));
	goomba->SetPosition(Vec2(fixedPos.x, fixedPos.y - height));
	goomba->objectState = make_shared<JumpingRedGoomba>(goomba);
	return goomba;
}
