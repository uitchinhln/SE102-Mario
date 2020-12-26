#include "RaccoonLeaf.h"
#include "Game.h"
#include "AnimationManager.h"
#include "MEntityType.h"
#include "Mario.h"
#include "SceneManager.h"

RaccoonLeaf::RaccoonLeaf()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	this->SetFacing(-1);

	this->GetGravity() = 0;
	this->Velocity = Vec2(0, Gravity);

	this->Distance = Velocity * (float)dt;
}

void RaccoonLeaf::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-super-mushroom");
	}
}

void RaccoonLeaf::CollisionUpdate(vector<shared_ptr<GameObject>>* coObj)
{
	vector<shared_ptr<GameObject>> objs;
	objs.push_back(SceneManager::GetInstance()->GetPlayer<Mario>());

	GetCollisionCalc()->CalcPotentialCollisions(&objs, false);
}

void RaccoonLeaf::PositionUpdate()
{
	UpdatedDistance = collisionCal->GetClampDistance();
	Position += UpdatedDistance;
}

void RaccoonLeaf::StatusUpdate()
{
	vector<shared_ptr<CollisionResult>> coResult = GetCollisionCalc()->GetLastResults();

	if (coResult.size() > 0) {
	/*	Vec2 jet = collisionCal->GetJet();

		if (jet.x != 0) Velocity.x = -Velocity.x;
		if (jet.y != 0) Velocity.y = 0;*/

		for each (shared_ptr<CollisionResult> coll in coResult)
		{
			if (MEntityType::IsMario(coll->Object->GetObjectType())) {
				SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
			}
		}
	}

	Vec2 mapBound = SceneManager::GetInstance()->GetActiveScene()->GetGameMap()->GetBound();
	if (GetPosition().x < 0.3 - size.x || GetPosition().y < 0.3 - size.y || GetPosition().x > mapBound.x || GetPosition().y > mapBound.y) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}
}

void RaccoonLeaf::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	shared_ptr<Mario> player;

	switch (movingStep)
	{
	case 0:
		if (Position.y + size.y + 144.0f <= rootPos.y) {
			Position.y = rootPos.y - size.y - 144.0f;
			Velocity.y = 0;
			movingStep = 1;
		}
		else {
			Velocity.y -= LEAF_GROWUP_SPEED * CGame::Time().ElapsedGameTime;
		}
		break;
	case 1:
		this->GetGravity() = LEAF_GRAVITY;
		Velocity.x = 0.03;
		break;
	default:
		break;
	}

	GetVelocity().y += GetGravity() * (float)dt;
	GetDistance() = GetVelocity() * (float)dt;
}

void RaccoonLeaf::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	facing = Velocity.x > 0 ? 1 : -1;
	collisionCal->Clear();
}

void RaccoonLeaf::Render()
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	Animation animation = this->animations["Default"];

	animation->GetTransform()->Position = GetPosition() - cam;
	animation->Render();
}

ObjectType RaccoonLeaf::GetObjectType()
{
	return MEntityType::RaccoonLeaf;
}

RectF RaccoonLeaf::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool RaccoonLeaf::IsGetThrough(GameObject& object, Direction direction)
{
	return true;
}

float RaccoonLeaf::GetDamageFor(GameObject& object, Direction direction)
{
	return 0.0f;
}
