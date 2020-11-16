#include "MarioTailed.h"
#include "SceneManager.h"

MarioTailed::MarioTailed(shared_ptr<Mario> holder, DWORD attackTime)
{
	this->holder = holder;
	this->attackTime = attackTime;
	MovingUpdate();
}

void MarioTailed::MovingUpdate()
{
	float speed = 138.0f / attackTime;
	if (shared_ptr<Mario> m = holder.lock()) {
		this->Position = m->GetPosition() + Vec2(0, 54);

		this->SetFacing(-1 * m->GetFacing() * attackState);

		if (this->GetFacing() > 0) {
			this->Position.x = m->GetHitBox().left;
		}
		else {
			this->Position.x = m->GetHitBox().right - 12;
		}

		Distance.x = 1.0f + speed * (attackTime / 2) * this->GetFacing();
		Distance.y = 0;
	}
}

void MarioTailed::Update()
{
	if (!attackTimer.IsRunning()) {
		attackTimer.Start();
	}
	MovingUpdate();	
	//DebugOut(L"Distance(%f, %f)\n", Distance.x, Distance.y);
	if (attackTimer.Elapsed() >= attackTime / 2) {
		if (attackState > 0) {
			attackState *= -1;
			attackTimer.Restart();
		}
		else {
			attackTimer.Stop();
			SceneManager::GetInstance()->GetActiveScene()->DespawnEntity(shared_from_this());
		}
	}
}

void MarioTailed::FinalUpdate()
{
	collisionCal->Clear();
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
	return RectF(Position.x, Position.y, Position.x + size.x, Position.y + size.y);
}

bool MarioTailed::IsGetThrough(IColliable& object, Direction direction)
{
	return false;
}

float MarioTailed::GetDamageFor(IColliable& object, Direction direction)
{
	return 2.0f;
}
