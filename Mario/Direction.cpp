#include "Direction.h"

Vec2 ToVector(Direction direction)
{
	switch (direction)
	{
	case Direction::Left:
		return Vec2(-1, 0);
		break;
	case Direction::Top:
		return Vec2(0, -1);
		break;
	case Direction::Right:
		return Vec2(1, 0);
		break;
	case Direction::Bottom:
		return Vec2(0, 1);
		break;
	case Direction::None:
		return Vec2(0, 0);
		break;
	default:
		return Vec2(0, 0);
		break;
	}
}

string GetDirectionName(Direction direction)
{
	switch (direction)
	{
	case Direction::Left:
		return "Left";
		break;
	case Direction::Top:
		return "Top";
		break;
	case Direction::Right:
		return "Right";
		break;
	case Direction::Bottom:
		return "Bottom";
		break;
	case Direction::None:
		return "None";
		break;
	default:
		return "";
		break;
	}
}

Direction Opposite(Direction input)
{
	{
		switch (input)
		{
		case Direction::Left:
			return Direction::Right;

		case Direction::Top:
			return Direction::Bottom;

		case Direction::Right:
			return Direction::Left;

		case Direction::Bottom:
			return Direction::Top;

		default:
			return Direction::None;
		}
	}
}
