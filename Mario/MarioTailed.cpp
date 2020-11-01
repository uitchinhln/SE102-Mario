#include "MarioTailed.h"
#include "SceneManager.h"

MarioTailed::MarioTailed(shared_ptr<Mario> holder, DWORD attackTime)
{
	this->holder = holder;
	this->attackTime = attackTime;
}

void MarioTailed::MovingUpdate()
{
	float speed = 3 * 14.0f / attackTime;
	if (shared_ptr<Mario> m = holder.lock()) {
		this->Position = m->GetPosition() + Vec2(-1, 54);

		this->SetFacing(-1 * m->GetFacing() * attackState);

		if (this->GetFacing() > 0) {
			this->Position.x = m->GetHitBox().right - 11;
		}

		Distance.x = 1 + speed * (attackState > 0 ? attackTime / 2 - attackTimer.Elapsed() : attackTimer.Elapsed()) * this->GetFacing();
		Distance.y = 0;
	}
}

void MarioTailed::Update(vector<shared_ptr<IColliable>>* coObj)
{
	if (!attackTimer.IsRunning()) {
		attackTimer.Start();
	}
	MovingUpdate();	
	DebugOut(L"Distance(%f, %f)\n", Distance.x, Distance.y);
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

void MarioTailed::Render()
{
}

ObjectType MarioTailed::GetObjectType()
{
	return MEntityType::MarioTailed;
}

RectF MarioTailed::GetHitBox()
{
	return RectF(Position.x, Position.y, Position.x + 12, Position.y + 12);
}

bool MarioTailed::IsGetThrough(IColliable& object, Direction direction)
{
	return true;
}

float MarioTailed::GetDamageFor(IColliable& object, Direction direction)
{
	return 2.0f;
}
