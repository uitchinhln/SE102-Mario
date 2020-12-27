#include "RayCast.h"

void RayCast::Shoot(Vec2 startPoint, Direction direction, float rayLength, ObjectList& results)
{
	results.clear();
	RectF hitbox;
	switch (direction)
	{
	case Direction::Left:
		for each (shared_ptr<GameObject> obj in *objects)
		{
			hitbox = obj->GetHitBox();
			if ((startPoint.x - rayLength <= hitbox.left && hitbox.left <= startPoint.x) ||
				(startPoint.x - rayLength <= hitbox.right && hitbox.right <= startPoint.x) ||
				(hitbox.left < startPoint.x && startPoint.x < hitbox.right)) {
				if (hitbox.top < startPoint.y && startPoint.y < hitbox.bottom) {
					results.push_back(obj);
				}
			}
		}
		break;
	case Direction::Top:
		for each (shared_ptr<GameObject> obj in *objects)
		{
			hitbox = obj->GetHitBox();
			if ((startPoint.y - rayLength <= hitbox.top && hitbox.top <= startPoint.y) ||
				(startPoint.y - rayLength <= hitbox.bottom && hitbox.bottom <= startPoint.y) ||
				(hitbox.top < startPoint.y && startPoint.y < hitbox.bottom)) {
				if (hitbox.left < startPoint.x && startPoint.x < hitbox.right) {
					results.push_back(obj);
				}
			}
		}
		break;
	case Direction::Right:
		for each (shared_ptr<GameObject> obj in *objects)
		{
			hitbox = obj->GetHitBox();
			if ((startPoint.x <= hitbox.left && hitbox.left <= startPoint.x + rayLength) ||
				(startPoint.x <= hitbox.right && hitbox.right <= startPoint.x + rayLength) ||
				(hitbox.left < startPoint.x && startPoint.x < hitbox.right)) {
				if (hitbox.top < startPoint.y && startPoint.y < hitbox.bottom) {
					results.push_back(obj);
				}
			}
		}
		break;
	case Direction::Bottom:
		for each (shared_ptr<GameObject> obj in *objects)
		{
			hitbox = obj->GetHitBox();
			if ((startPoint.y <= hitbox.top && hitbox.top <= startPoint.y + rayLength) ||
				(startPoint.y <= hitbox.bottom && hitbox.bottom <= startPoint.y + rayLength) ||
				(hitbox.top < startPoint.y && startPoint.y < hitbox.bottom)) {
				if (hitbox.left < startPoint.x && startPoint.x < hitbox.right) {
					results.push_back(obj);
				}
			}
		}
		break;
	}
}

void RayCast::MergeBox(ObjectList& input, Direction direction, float maxSpace, vector<shared_ptr<Vec2>>& output)
{
	output.clear();
	if (input.size() > 0) {
		if (ToVector(direction).x != 0) {
			sort(input.begin(), input.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
				return a->GetHitBox().left < b->GetHitBox().left;
				}
			);

			output.push_back(make_shared<Vec2>(input.at(0)->GetHitBox().left, input.at(0)->GetHitBox().right));

			for each (shared_ptr<GameObject> o in input)
			{
				shared_ptr<Vec2> last = output.back();
				RectF hitbox = o->GetHitBox();

				if (last->y + maxSpace >= hitbox.left) {
					last->y = max(hitbox.right, last->y);
				}
				else {
					output.push_back(make_shared<Vec2>(hitbox.left, hitbox.right));
				}
			}
		}
		else {
			sort(input.begin(), input.end(), [](shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
				return a->GetHitBox().top < b->GetHitBox().top;
				}
			);

			output.push_back(make_shared<Vec2>(input.at(0)->GetHitBox().top, input.at(0)->GetHitBox().bottom));

			for each (shared_ptr<GameObject> o in input)
			{
				shared_ptr<Vec2> last = output.back();
				RectF hitbox = o->GetHitBox();

				if (last->y + maxSpace >= hitbox.top) {
					last->y = max(hitbox.bottom, last->y);
				}
				else {
					output.push_back(make_shared<Vec2>(hitbox.top, hitbox.bottom));
				}
			}
		}
	}
}

void RayCast::SetInput(ObjectList* objects)
{
	this->objects = objects;
}
