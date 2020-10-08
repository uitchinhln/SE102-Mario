#include "Position.h"

Position::Position(int x, int y)
{
	SetX(x);
	SetY(y);
}

int Position::GetX()
{
	return this->x;
}

void Position::SetX(int x)
{
	this->x = x;
}

int Position::GetY()
{
	return this->y;
}

void Position::SetY(int y)
{
	this->y = y;
}
