#pragma once
#include <math.h>

using namespace std;

class Vec2
{
private:
	float x;
	float y;
public:
	Vec2() {
		this->x = 0;
		this->y = 0;
	}

	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void SetX(float x);

	float GetX();

	void SetY(float y);

	float GetY();

	float ToAbsoluteValue();

	void SetAbsoluteValue(float value);

	Vec2 operator+(const Vec2&);
	Vec2 operator-(const Vec2&);
	Vec2 operator*(const Vec2&);
	Vec2 operator/(const Vec2&);
};

