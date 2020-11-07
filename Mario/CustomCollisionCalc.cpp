#include "CustomCollisionCalc.h"

CustomCollisionCalc::CustomCollisionCalc(weak_ptr<IColliable> object) : CollisionCalculator(object)
{
}

SweptCollisionResult CustomCollisionCalc::SweptAABB(RectF m, Vec2 distance, RectF s, bool debug)
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
		return SweptCollisionResult::Empty();

	if (debug) {
		DebugOut(L"1Not Exit\n");
	}

	if (distance.x == 0 && distance.y == 0)
		return SweptCollisionResult::Empty();

	if (debug) {
		DebugOut(L"2Not Exit\n");
	}

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
		return SweptCollisionResult::Empty();

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (debug) {
		DebugOut(L"tx_exit=%f\ty_exit=%f\n", tx_exit, ty_exit);
		DebugOut(L"t_entry=%f\t_exit=%f\n", t_entry, t_exit);
	}

	if (t_entry > t_exit)
		return SweptCollisionResult::Empty();

	if (debug) {
		DebugOut(L"3Not Exit\n");
	}

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

	// SAT test
	CPolygon c1;
	c1.vertices.push_back(Vec2(m.left, m.top));
	c1.vertices.push_back(Vec2(m.right, m.top));
	c1.vertices.push_back(Vec2(m.right, m.bottom));
	c1.vertices.push_back(Vec2(m.left, m.bottom));
	CPolygon c2;
	c2.vertices.push_back(Vec2(s.left, s.top));
	c2.vertices.push_back(Vec2(s.right, s.top));
	c2.vertices.push_back(Vec2(s.right, s.bottom));
	c2.vertices.push_back(Vec2(s.left, s.bottom));

	c1.BuildEdge();
	c2.BuildEdge();

	Vec2 perpVel = Vec2Utils::Normalize(Vec2(abs(distance.y), abs(distance.x)));

	float minA = 99999, minB = 99999, maxA = 0, maxB = 0;

	c1.Project(perpVel, minA, maxA);
	c2.Project(perpVel, minB, maxB);

	if (debug) {
		DebugOut(L"perpVel: %f\t%f\t\n", perpVel.x, perpVel.y);
		DebugOut(L"Project: %f\t%f\t%f\t%f\t\n", minA, maxA, minB, maxB);
		DebugOut(L"Project: %f\n", (minA <= minB ? minB - maxA : minA - maxB));
	}
	if (direction == Direction::Top && (minA <= minB ? minB - maxA : minA - maxB) > -min((m.right - m.left), (s.right - s.left)) / 1.9) {
		return SweptCollisionResult::Empty();
	}

	return SweptCollisionResult{ t_entry, direction, distance };
}
