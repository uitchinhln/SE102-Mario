#include "AbstractMarioState.h"
#include "Mario.h"

RectF AbstractMarioState::GetBoundingBox()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 pos = m->GetPosition();
		return RectF(pos.x, pos.y, pos.x + 15 * 3, pos.y + 27 * 3);
	}
	return RectF(0, 0, 0, 0);
}

void AbstractMarioState::Update(vector<shared_ptr<CollisionResult>> collisions)
{
}

void AbstractMarioState::Render()
{
}
