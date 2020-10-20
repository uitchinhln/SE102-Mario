#pragma once
#include "Utils.h"

enum class Direction
{
	Left,
	Top,
	Right,
	Bottom,
	None,
};

Vec2 ToVector(Direction direction);

string GetDirectionName(Direction direction);

Direction Opposite(Direction input);

