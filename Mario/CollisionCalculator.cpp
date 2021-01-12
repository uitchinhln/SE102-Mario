#include "CollisionCalculator.h"
#include "GameObject.h"
#include "MEntityType.h"

CollisionCalculator::CollisionCalculator(weak_ptr<GameObject> object)
{
    this->object = object;
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::CalcPotentialCollisions(vector<shared_ptr<GameObject>>* objects, bool debug)
{
	vector<shared_ptr<CollisionResult>> temp;
	temp.clear();
	results.clear();
	overlaps.clear();
	_results.clear();
	key_results.clear();

	if (shared_ptr<GameObject> sp = object.lock()) {
		RectF spR = sp->GetHitBox();
		Vec2 spD = sp->GetDistance();

		//auto start = std::chrono::high_resolution_clock::now();
		for each (shared_ptr<GameObject> coO in (*objects))
		{		
			if (!coO->IsActive() || sp->GetID() == coO->GetID()) continue;

			RectF cpR = coO->GetHitBox();
			Vec2 cpD = coO->GetDistance();

			SweptCollisionResult aabbResult = SweptAABB(spR, spD - cpD, cpR, debug);

			switch (aabbResult.Status)
			{
			case CollisionStatus::COLLIDE:
				temp.push_back(make_shared<CollisionResult>(aabbResult, coO));
				break;
			case CollisionStatus::OUT_OF_RANGE:
				if (!BroadPhase(spR, spD, cpR) && !BroadPhase(cpR, cpD, spR)) continue;
				_results.push_back(make_shared<CollisionResult>(aabbResult, coO));
				break;
			case CollisionStatus::OVERLAP:
				if (coO->IsGetThrough(*sp, aabbResult.Direction)) break;
				overlaps.push_back(make_shared<CollisionResult>(aabbResult, coO));
				break;
			default:
				break;
			}
		}
		sort(temp.begin(), temp.end(), CollisionResult::LPCompare);
		//auto finish = std::chrono::high_resolution_clock::now();
		//DebugOut(L"%s\t%d\n", ToLPCWSTR(sp->GetObjectType().ToString()), std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());

		for each (shared_ptr<CollisionResult> coll in temp) {
			for each (shared_ptr<CollisionResult> result in results) {
				if (result->Object->IsGetThrough(*sp, result->SAABBResult.Direction)) continue;
				Vec2 dis = spD - coll->Object->GetDistance();
				if (ToVector(coll->SAABBResult.Direction).x != 0) {
					dis.y = dis.y * result->SAABBResult.TimeToCollide;
				}
				else {
					dis.x = dis.x * result->SAABBResult.TimeToCollide;
				}
				SweptCollisionResult aabbResult = SweptAABB(sp->GetHitBox(), dis, coll->Object->GetHitBox());
				if (aabbResult.Status != CollisionStatus::COLLIDE)
					coll->SAABBResult.Status = aabbResult.Status;
			}
			if (coll->SAABBResult.Status == CollisionStatus::COLLIDE) {
				results.push_back(coll);
				key_results[coll->Object->GetID()] = coll;
			}
		}
	}	
	GetClampDistance();
	return results;
}

void CollisionCalculator::RestoreCollision()
{
	if (_results.size() <= 0) return;

	if (shared_ptr<GameObject> sp = object.lock()) {
		RectF spR = sp->GetHitBox();
		RectF _spR = spR;

		Vec2 spD = sp->GetUpdatedDistance();
		spR.left -= spD.x;
		spR.right -= spD.x;
		spR.top -= spD.y;
		spR.bottom -= spD.y;

		for each (shared_ptr<CollisionResult> var in _results)
		{
			RectF cpR = var->Object->GetHitBox();

			if (AABB(_spR, cpR)) {
				Vec2 cpD = var->Object->GetUpdatedDistance();
				cpR.left -= cpD.x;
				cpR.right -= cpD.x;
				cpR.top -= cpD.y;
				cpR.bottom -= cpD.y;

				SweptCollisionResult aabbResult = SweptAABB(spR, spD - cpD, cpR);

				if (aabbResult.Status == CollisionStatus::COLLIDE) {
					var->SAABBResult = aabbResult;
					results.push_back(var);
					key_results[var->Object->GetID()] = var;
				}
			}
		}
		sort(results.begin(), results.end(), CollisionResult::LPCompare);
	}	
	GetClampDistance();
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::GetLastResults()
{
	return results;
}

vector<shared_ptr<CollisionResult>> CollisionCalculator::GetOverlaps()
{
	return overlaps;
}

int CollisionCalculator::HasOverlapped()
{
	return overlaps.size();
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
			if (c->Object->IsGetThrough(*sp, c->SAABBResult.Direction)) continue;
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
	_results.clear();
}

void CollisionCalculator::DropRemovedCollision()
{
	if (shared_ptr<GameObject> sp = object.lock()) {
		results.erase(remove_if(results.begin(), results.end(), [this](const shared_ptr<CollisionResult>& coll) {
			if (coll->Remove) {
				key_results.erase(coll->Object->GetID());
				return true;
			}
			return false;
			}), results.end());
	}
}

SweptCollisionResult CollisionCalculator::SweptAABB(RectF m, Vec2 distance, RectF s, bool debug)
{
	float dx_entry = 0, dx_exit, tx_entry, tx_exit;
	float dy_entry = 0, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	if (!BroadPhase(m, distance, s))
		return SweptCollisionResult{ -1, Direction::None, distance, 0.0f,  CollisionStatus::OUT_OF_RANGE };

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

	if (tx_entry < 0.0f && ty_entry < 0.0f)
		return SweptCollisionResult::Overlap;

	if (tx_entry > 1.0f || ty_entry > 1.0f)
		return SweptCollisionResult{ -1, Direction::None, distance, 0.0f,  CollisionStatus::OUT_OF_RANGE };

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit)
		return SweptCollisionResult{ -1, Direction::None, distance, 0.0f,  CollisionStatus::OUT_OF_RANGE };

	Direction direction = Direction::None;
	float touchingLength = 0;

	if (tx_entry > ty_entry)
	{
		if (distance.x > 0)
			direction = Direction::Left;
		else
			direction = Direction::Right;

		float mst = m.top + distance.y * t_entry;
		float msb = m.bottom + distance.y * t_entry;
		touchingLength = min(msb, s.bottom) - max(mst, s.top);
	}
	else
	{
		if (distance.y > 0)
			direction = Direction::Top;
		else
			direction = Direction::Bottom;

		float msl = m.left + distance.x * t_entry;
		float msr = m.right + distance.x * t_entry;
		touchingLength = (min(msr, s.right) - max(msl, s.left));
	}

	if (touchingLength <= 0) 
		return SweptCollisionResult::Empty;

	return SweptCollisionResult{ t_entry, direction, distance, touchingLength };
}

bool CollisionCalculator::AABB(RectF b1, RectF b2)
{
	return !(b1.right <= b2.left || b1.left >= b2.right || b1.top >= b2.bottom || b1.bottom <= b2.top);
}

bool CollisionCalculator::BroadPhase(RectF movingBounding, Vec2 distance, RectF staticBounding)
{
	float bl = distance.x > 0 ? movingBounding.left : movingBounding.left + distance.x;
	float bt = distance.y > 0 ? movingBounding.top : movingBounding.top + distance.y;
	float br = distance.x > 0 ? movingBounding.right + distance.x : movingBounding.right;
	float bb = distance.y > 0 ? movingBounding.bottom + distance.y : movingBounding.bottom;

	return !(br <= staticBounding.left || bl >= staticBounding.right || bb <= staticBounding.top || bt >= staticBounding.bottom);
}

bool CollisionCalculator::Has(DWORD id)
{
	return key_results.size() > 0 && key_results.find(id) != key_results.end();
}
