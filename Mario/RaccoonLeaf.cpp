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
	this->Visible = false;
}

void RaccoonLeaf::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-super-leaf-red");
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
			if (!this->Visible && Position.y + 10.0f <= rootPos.y) {
				this->Visible = true;
			}
			Velocity.y -= LEAF_GROWUP_SPEED * CGame::Time().ElapsedGameTime;
		}
		break;
	case 1:
		this->GetGravity() = LEAF_GRAVITY;
		this->renderOrder = 1500;

		if (abs(Position.x - rootPos.x) >= LEAF_REVEAL_DISTANCE) {
			facing = -facing;
		}

		Velocity.x = LEAF_REVEAL_FORCE * dt * facing;

		break;
	default:
		break;
	}

	Velocity.y = min(Velocity.y + Gravity * dt, LEAF_FALLING_VEL);
	GetDistance() = GetVelocity() * (float)dt;


}

void RaccoonLeaf::FinalUpdate()
{
	Distance = Velocity * (float)CGame::Time().ElapsedGameTime;
	//facing = Velocity.x > 0 ? 1 : -1;
	collisionCal->Clear();
}

void RaccoonLeaf::Render()
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	Animation animation = this->animations["Default"];

	animation->GetTransform()->Scale.x = -facing;
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

shared_ptr<RaccoonLeaf> RaccoonLeaf::CreateRaccoonLeaf(Vec2 pos)
{
	shared_ptr<RaccoonLeaf> leaf = make_shared<RaccoonLeaf>();
	leaf->SetCollisionCalculator(make_shared<CollisionCalculator>(leaf));
	leaf->SetPosition(pos);
	leaf->rootPos = pos;
	return leaf;
}
