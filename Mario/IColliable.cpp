#include "IColliable.h"
#include "CollisionResult.h"

SweptAABBResult IColliable::SweptAABB(RectF m, Vec2 distance, RectF s)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

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


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f)
		return SweptAABBResult::Empty();

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

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
	return SweptAABBResult{ t_entry, direction };
}
