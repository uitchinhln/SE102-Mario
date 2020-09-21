#include "Vec2.h"

void Vec2::SetX(float x)
{
	this->x = x;
}

float Vec2::GetX()
{
	return this->x;
}

void Vec2::SetY(float y)
{
	this->y = y;
}

float Vec2::GetY()
{
	return this->y;
}

float Vec2::ToAbsoluteValue()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

void Vec2::SetAbsoluteValue(float value)
{
	float _Scale = value / this->ToAbsoluteValue();
	this->SetX(this->GetX() * _Scale);
	this->SetY(this->GetY() * _Scale);
}

Vec2 Vec2::operator+(const Vec2&)
{
	return Vec2();
}
