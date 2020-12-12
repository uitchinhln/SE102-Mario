#include "MarioTailed.h"
#include "SceneManager.h"

MarioTailed::MarioTailed(shared_ptr<Mario> holder, DWORD attackTime)
{
	this->holder = holder;
	this->attackTime = attackTime;
	this->speed = 138.0f / attackTime;
	MovingUpdate();
}

void MarioTailed::CollisionDoubleFilter()
{
}

bool MarioTailed::HasCollideWith(DWORD id)
{
	return true;
}

void MarioTailed::MovingUpdate()
{
	if (shared_ptr<Mario> m = holder.lock()) {
		this->Position = m->GetPosition() + Vec2(0, 54);

		this->SetFacing(m->GetFacing() * attackState);

		if (this->GetFacing() > 0) {
			this->Position.x = m->GetHitBox().left;
		}
		else {
			this->Position.x = m->GetHitBox().right - 12;
		}

		Distance.x = 1.0f + speed * (attackTime >> 1) * this->GetFacing();
		Distance.y = 0;
		UpdatedDistance = Distance;
	}
}

void MarioTailed::Update()
{
	if (!attackTimer.IsRunning()) {
		attackTimer.Start();
	}
	MovingUpdate();	

	if (attackTimer.Elapsed() >= attackTime >> 1) {
		if (attackState < 0) {
			attackState *= -1;
			attackTimer.Restart();
		}
		else {
			attackTimer.Stop();
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
		}
	}
}

void MarioTailed::PositionUpdate()
{
}

void MarioTailed::PositionLateUpdate()
{
}

void MarioTailed::FinalUpdate()
{
	//collisionCal->Clear();
}

void MarioTailed::Render()
{
}

ObjectType MarioTailed::GetObjectType()
{
	return MEntityType::MarioTailed;
}

RectF MarioTailed::GetHitBox()
{
	hitbox.left = Position.x;
	hitbox.top = Position.y;
	hitbox.right = Position.x + size.x;
	hitbox.bottom = Position.y + size.y;
	return hitbox;
}

bool MarioTailed::IsGetThrough(GameObject& object, Direction direction)
{
	return false;
}

float MarioTailed::GetDamageFor(GameObject& object, Direction direction)
{
	return 2.0f;
}

MarioTailed::~MarioTailed()
{
	//DebugOut(L"Huy mario tail\n");
}
