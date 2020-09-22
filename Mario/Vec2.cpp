#include "Vec2.h"

void CVec2::SetX(float x)
{
	this->x = x;
}

float CVec2::GetX()
{
	return this->x;
}

void CVec2::SetY(float y)
{
	this->y = y;
}

float CVec2::GetY()
{
	return this->y;
}

float CVec2::ToAbsoluteValue()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

void CVec2::SetAbsoluteValue(float value)
{
	if (this->x + this->y == 0) return;
	float _Scale = value / this->ToAbsoluteValue();
	this->SetX(this->GetX() * _Scale);
	this->SetY(this->GetY() * _Scale);
}

CVec2* CVec2::Add(CVec2* vec)
{
	this->x += vec->GetX();
	this->y += vec->GetY();
	return this;
}

CVec2* CVec2::Subtract(CVec2* vec)
{
	this->x -= vec->GetX();
	this->y -= vec->GetY();
	return this;
}

CVec2* CVec2::Multipli(float k)
{
	this->x *= k;
	this->y *= k;
	return this;
}
