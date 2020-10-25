#include "CollisionCalculator.h"

CollisionCalculator::CollisionCalculator(weak_ptr<IColliable> object)
{
    this->object = object;
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::CalcPotentialCollisions(vector<shared_ptr<IColliable>>* objects, bool debug)
{
	vector<shared_ptr<CollisionResult>> temp;
	temp.clear();
	results.clear();

	if (shared_ptr<IColliable> sp = object.lock()) {
		for each (shared_ptr<IColliable> coO in (*objects))
		{
			SweptAABBResult aabbResult = SweptAABB(sp->GetHitBox(), sp->GetDistance() - coO->GetDistance(), coO->GetHitBox(), debug);
			shared_ptr<CollisionResult> result = make_shared<CollisionResult>(aabbResult, coO);

			if (debug)
				DebugOut(L"direction = %s\t t=%f\n", ToLPCWSTR(GetDirectionName(result->SAABBResult.Direction)), result->SAABBResult.TimeToCollide);

			if (result->SAABBResult.TimeToCollide > 0 && result->SAABBResult.TimeToCollide <= 1.0f)
				temp.push_back(result);
		}
		sort(temp.begin(), temp.end(), CollisionResult::LPCompare);

		for each (shared_ptr<CollisionResult> coll in temp) {
			for each (shared_ptr<CollisionResult> result in results) {
				if (result->GameColliableObject->IsGetThrough(*sp, result->SAABBResult.Direction)) continue;
				Vec2 dis = (sp->GetDistance() - coll->GameColliableObject->GetDistance());
				if (ToVector(coll->SAABBResult.Direction).x != 0) {
					dis.y *= result->SAABBResult.TimeToCollide;
					dis.y -= 0.1;
				}
				else {
					dis.x *= result->SAABBResult.TimeToCollide;
					dis.x -= 0.1;
				}
				SweptAABBResult aabbResult = SweptAABB(sp->GetHitBox(), dis, coll->GameColliableObject->GetHitBox());
				if (aabbResult.TimeToCollide <= 0 || aabbResult.TimeToCollide > 1.0f) {
					coll->SAABBResult.TimeToCollide = 9999.0f;
					break;
				}
			}
			if (coll->SAABBResult.TimeToCollide > 0 && coll->SAABBResult.TimeToCollide <= 1.0f) {
				results.push_back(coll);
			}
			else {
				if (debug)
					DebugOut(L"Da loai va cham %s khong hop le\n", ToLPCWSTR(GetDirectionName(coll->SAABBResult.Direction)));
			}
		}
		sort(results.begin(), results.end(), CollisionResult::LPCompare);
	}	
	GetNewDistance();
	return results;
}

Vec2 CollisionCalculator::GetNewDistance()
{
	if (shared_ptr<IColliable> sp = object.lock()) {
		Vec2 d = sp->GetDistance();
		float min_tx = 1.0f;
		float min_ty = 1.0f;

		float nx = 0.0f;
		float ny = 0.0f;
		float rdx = 0.0f;
		float rdy = 0.0f;

		for each (shared_ptr<CollisionResult> c in results)
		{
			if (c->GameColliableObject->IsGetThrough(*sp, c->SAABBResult.Direction)) continue;
			Vec2 cn = ToVector(c->SAABBResult.Direction);

			if (c->SAABBResult.TimeToCollide < min_tx && cn.x != 0) {
				min_tx = c->SAABBResult.TimeToCollide;
				nx = cn.x;
				rdx = c->SAABBResult.Distance.x;
			}

			if (c->SAABBResult.TimeToCollide < min_ty && cn.y != 0) {
				min_ty = c->SAABBResult.TimeToCollide;
				ny = cn.y;
				rdy = c->SAABBResult.Distance.y;
			}
		}
		jet = Vec2(nx, ny);
		return Vec2(min_tx * d.x + nx * 0.002f, min_ty * d.y + ny * 0.002f);
		/*float min_t = min_tx < min_ty ? min_tx : min_ty;
		return Vec2(min_t * d.x + nx * 0.4f, min_t * d.y + ny * 0.4f);*/
	}
	return VECTOR_0;
}

Vec2 CollisionCalculator::GetJet()
{
	return jet;
}

SweptAABBResult CollisionCalculator::SweptAABB(RectF m, Vec2 distance, RectF s, bool debug)
{
	float dx_entry = 0, dx_exit, tx_entry, tx_exit;
	float dy_entry = 0, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	if (debug) {
		DebugOut(L"Moving bounding: top=%f\tleft=%f\tbottom=%f\tright=%f\t\n", m.top, m.left, m.bottom, m.right);
		DebugOut(L"Static bounding: top=%f\tleft=%f\tbottom=%f\tright=%f\t\n", s.top, s.left, s.bottom, s.right);
		DebugOut(L"Distance: x=%f\ty=%f\n", distance.x, distance.y);
	}

	//
	// Broad-phase test 
	//

	float bl = distance.x > 0 ? m.left : m.left + distance.x;
	float bt = distance.y > 0 ? m.top : m.top + distance.y;
	float br = distance.x > 0 ? m.right + distance.x : m.right;
	float bb = distance.y > 0 ? m.bottom + distance.y : m.bottom;

	if (br < s.left || bl > s.right || bb < s.top || bt > s.bottom)
		return SweptAABBResult::Empty();


	if (distance.x == 0 && distance.y == 0)
		return SweptAABBResult::Empty();		// moving object is not moving > obvious no collision

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

	if (debug) {
		DebugOut(L"dx_entry=%f\tdy_entry=%f\n", dx_entry, dy_entry);
		DebugOut(L"tx_entry=%f\tty_entry=%f\n", tx_entry, ty_entry);
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f)
		return SweptAABBResult::Empty();

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (debug) {
		DebugOut(L"tx_exit=%f\ty_exit=%f\n", tx_exit, ty_exit);
		DebugOut(L"t_entry=%f\t_exit=%f\n", t_entry, t_exit);
	}

	if (t_entry > t_exit)
		return SweptAABBResult::Empty();

	Direction direction = Direction::None;

	if (tx_entry > ty_entry)
	{
		if (distance.x > 0)
			direction = Direction::Left;
		else
			direction = Direction::Right;
	}
	else
	{
		if (distance.y > 0)
			direction = Direction::Top;
		else
			direction = Direction::Bottom;
	}
	return SweptAABBResult{ t_entry, direction, distance };
}
