#include "MarioFireBall.h"
#include "AnimationManager.h"
#include "SceneManager.h"
#include "MEntityType.h"

MarioFireBall::MarioFireBall(shared_ptr<Mario> holder)
{
	this->holder = holder;

	Reset();
}

void MarioFireBall::InitResource()
{
	if (this->animations.size() < 1) {
		this->animations["Default"] = AnimationManager::GetInstance()->Get("ani-fire-ball")->Clone();
	}
}

void MarioFireBall::Reset()
{
	DWORD dt = CGame::Time().ElapsedGameTime;
	if (shared_ptr<Mario> m = holder.lock()) {
		this->Position.x = m->GetHitBox().left + size.x / 2;
		this->Position.y = m->GetHitBox().top + 30;
		this->SetFacing(m->GetFacing());

		this->Gravity = FIREBALL_GRAVITY;
		this->Velocity = Vec2(FIREBALL_SPEED * facing, 0);

		this->Distance = Velocity * dt;
	}
}

void MarioFireBall::CollisionUpdate(vector<shared_ptr<IColliable>>* coObj)
{
	shared_ptr<CollisionCalculator> collisionCal = GetCollisionCalc();

	vector<shared_ptr<CollisionResult>> coResult = collisionCal->CalcPotentialCollisions(coObj, false);

	// No collision occured, proceed normally
	if (coResult.size() == 0)
	{
		//GetPosition() += GetDistance();
	}
	else {
		//GetDistance() = collisionCal->GetNewDistance();
		Vec2 jet = collisionCal->GetJet();
		//GetPosition() += GetDistance();

		if (jet.x != 0) GetVelocity().x = 0;
		if (jet.y != 0) GetVelocity().y = jet.y * FIREBALL_JUMP_FORCE;
	}

	RectF cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->GetBoundingBox();
	if (Position.x < cam.left || Position.y < cam.top || Position.x > cam.right || Position.y > cam.bottom) {
		SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
	}

	for each (shared_ptr<CollisionResult> coll in coResult)
	{
		if (MEntityType::IsMario(coll->GameColliableObject->GetObjectType()) || MEntityType::IsMarioWeapon(coll->GameColliableObject->GetObjectType())) continue;
		if (!coll->GameColliableObject->IsGetThrough(*this, coll->SAABBResult.Direction)) {
			if (!MEntityType::IsTile(coll->GameColliableObject->GetObjectType()) || coll->SAABBResult.Direction != Direction::Top) {
				SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
			}
		}		
	}
}

void MarioFireBall::Update()
{
	DWORD dt = CGame::Time().ElapsedGameTime;

	GetVelocity().y += GetGravity() * dt;
	GetDistance() = GetVelocity() * dt;
}

void MarioFireBall::FinalUpdate()
{
	GameObject::FinalUpdate();
	collisionCal->Clear();
}

void MarioFireBall::Render()
{
	InitResource();

	Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;

	this->animations["Default"]->GetTransform()->Position = GetPosition() - cam;
	this->animations["Default"]->Render();
}

ObjectType MarioFireBall::GetObjectType()
{
	return MEntityType::MarioFireBall;
}

RectF MarioFireBall::GetHitBox()
{
	return RectF(this->Position.x, this->Position.y, this->Position.x + size.x, this->Position.y + size.y);
}

bool MarioFireBall::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float MarioFireBall::GetDamageFor(IColliable& object, Direction direction)
{
	return 3.0f;
}
