#include "CollisionCalculator.h"
#include "GameObject.h"

CollisionCalculator::CollisionCalculator(weak_ptr<GameObject> object)
{
    this->object = object;
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::CalcPotentialCollisions(vector<shared_ptr<GameObject>>* objects, bool debug)
{
	vector<shared_ptr<CollisionResult>> temp;
	temp.clear();
	results.clear();
	key_results.clear();

	if (shared_ptr<GameObject> sp = object.lock()) {
		//auto start = std::chrono::high_resolution_clock::now();
		for each (shared_ptr<GameObject> coO in (*objects))
		{
			if (!coO->IsActive()) continue;

			SweptCollisionResult aabbResult = SweptAABB(sp->GetHitBox(), sp->GetDistance() - coO->GetDistance(), coO->GetHitBox(), debug);

			if (aabbResult.TimeToCollide >= 0 && aabbResult.TimeToCollide < 1.0f)
				temp.push_back(make_shared<CollisionResult>(aabbResult, coO));
		}
		//auto finish = std::chrono::high_resolution_clock::now();
		//DebugOut(L"%s\t%d\n", ToLPCWSTR(sp->GetObjectType().ToString()), std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
		sort(temp.begin(), temp.end(), CollisionResult::LPCompare);

		for each (shared_ptr<CollisionResult> coll in temp) {
			for each (shared_ptr<CollisionResult> result in results) {
				if (result->GameColliableObject->IsGetThrough(*sp, result->SAABBResult.Direction)) continue;
				Vec2 dis = sp->GetDistance() - coll->GameColliableObject->GetDistance();
				Vec2 drctn = ToVector(coll->SAABBResult.Direction);
				if (drctn.x != 0) {
					dis.y = dis.y * result->SAABBResult.TimeToCollide - 0.0001f * drctn.y;
				}
				else {
					dis.x = dis.x * result->SAABBResult.TimeToCollide - 0.0001f * drctn.x;
				}
				SweptCollisionResult aabbResult = SweptAABB(sp->GetHitBox(), dis, coll->GameColliableObject->GetHitBox());
				if (aabbResult.TimeToCollide < 0 || aabbResult.TimeToCollide > 1.0f) {
					coll->SAABBResult.TimeToCollide = 9999.0f;
					break;
				}
			}
			if (coll->SAABBResult.TimeToCollide >= 0 && coll->SAABBResult.TimeToCollide <= 1.0f) {
				results.push_back(coll);
				key_results[coll->GameColliableObject->GetID()] = coll;
			}
		}
	}	
	GetClampDistance();
	return results;
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::GetLastResults()
{
	return results;
}

Vec2 CollisionCalculator::GetClampDistance()
{
	if (shared_ptr<GameObject> sp = object.lock()) {
		Vec2 d = sp->GetDistance();
		float min_tx = 1.0f;
		float min_ty = 1.0f;

		jet.x = 0.0f;
		jet.y = 0.0f;

		for each (shared_ptr<CollisionResult> c in results)
		{
			if (c->GameColliableObject->IsGetThrough(*sp, c->SAABBResult.Direction)) continue;
			Vec2 cn = ToVector(c->SAABBResult.Direction);

			if (c->SAABBResult.TimeToCollide < min_tx && cn.x != 0 && cn.x * d.x < 0.0f) {
				min_tx = c->SAABBResult.TimeToCollide;
				jet.x = cn.x;
			}

			if (c->SAABBResult.TimeToCollide < min_ty && cn.y != 0 && cn.y * d.y < 0.0f) {
				min_ty = c->SAABBResult.TimeToCollide;
				jet.y = cn.y;
			}
		}
		return Vec2(min_tx * d.x, min_ty * d.y);
	}
	return VECTOR_0;
}

Vec2 CollisionCalculator::GetJet()
{
	return jet;
}

void CollisionCalculator::Clear()
{
	results.clear();
	key_results.clear();
}

void CollisionCalculator::DropRemovedCollision()
{
	results.erase(remove_if(results.begin(), results.end(), [this](const shared_ptr<CollisionResult>& coll) {
		if (coll->Remove) {
			key_results.erase(coll->GameColliableObject->GetID());
		}
		return coll->Remove;
		}), results.end());

}

SweptCollisionResult CollisionCalculator::SweptAABB(RectF m, Vec2 distance, RectF s, bool debug)
{
	float dx_entry = 0, dx_exit, tx_entry, tx_exit;
	float dy_entry = 0, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;
	
	// Broad-phase test 
	float bl = distance.x > 0 ? m.left : m.left + distance.x;
	float bt = distance.y > 0 ? m.top : m.top + distance.y;
	float br = distance.x > 0 ? m.right + distance.x : m.right;
	float bb = distance.y > 0 ? m.bottom + distance.y : m.bottom;

	if (br < s.left || bl > s.right || bb < s.top || bt > s.bottom)
		return SweptCollisionResult::Empty;

	if (distance.x == 0 && distance.y == 0)
		return SweptCollisionResult::Empty;

	if (distance.x > 0)
	{
		dx_entry = s.left - m.right;
		dx_exit = s.right - m.left;
	}
	else if (distance.x < 0)
	{
		dx_entry = s.right - m.left;
		dx_exit = s.left - m.right;
	}


	if (distance.y > 0)
	{
		dy_entry = s.top - m.bottom;
		dy_exit = s.bottom - m.top;
	}
	else if (distance.y < 0)
	{
		dy_entry = s.bottom - m.top;
		dy_exit = s.top - m.bottom;
	}

	if (distance.x == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / distance.x;
		tx_exit = dx_exit / distance.x;
	}

	if (distance.y == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / distance.y;
		ty_exit = dy_exit / distance.y;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f)
		return SweptCollisionResult::Empty;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit)
		return SweptCollisionResult::Empty;

	Direction direction = Direction::None;
	float touchingLength = 0;

	if (tx_entry > ty_entry)
	{
		if (distance.x > 0)
			direction = Direction::Left;
		else
			direction = Direction::Right;

		Vec2 mAfter(m.top + distance.y * t_entry, m.bottom + distance.y * t_entry);
		touchingLength = (min(mAfter.y, s.bottom) - max(mAfter.x, s.top));
	}
	else
	{
		if (distance.y > 0)
			direction = Direction::Top;
		else
			direction = Direction::Bottom;

		Vec2 mAfter(m.left + distance.x * t_entry, m.right + distance.x * t_entry);
		touchingLength = (min(mAfter.y, s.right) - max(mAfter.x, s.left));
	}
	if (touchingLength <= 0) 
		return SweptCollisionResult::Empty;

	return SweptCollisionResult{ t_entry, direction, distance, touchingLength };
}

bool CollisionCalculator::AABB(RectF b1, RectF b2)
{
	return !(b1.right < b2.left || b1.left > b2.right || b1.top > b2.bottom || b1.bottom < b2.top);
}

bool CollisionCalculator::Has(DWORD64 id)
{
	return key_results.size() > 0 && key_results.find(id) != key_results.end();
}
