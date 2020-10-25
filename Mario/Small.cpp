#include "Small.h"
#include "Mario.h"

Small::Small(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{

}

RectF Small::GetBoundingBox()
{
	if (shared_ptr<Mario> m = mario.lock()) {
		Vec2 pos = m->GetPosition();
		return RectF(pos.x, pos.y, pos.x + 36, pos.y + 45);
	}
	return RectF(0, 0, 0, 0);
}
