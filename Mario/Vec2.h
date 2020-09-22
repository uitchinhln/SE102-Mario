#pragma once
#include <math.h>

using namespace std;

class CVec2
{
private:
	float x;
	float y;
public:
	CVec2() {
		this->x = 0;
		this->y = 0;
	}

	CVec2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void SetX(float x);

	float GetX();

	void SetY(float y);

	float GetY();

	float ToAbsoluteValue();

	void SetAbsoluteValue(float value);

	CVec2* Add(CVec2* vec);

	CVec2* Subtract(CVec2* vec);

	CVec2* Multipli(float k);

	//CVec2* Multipli(CVec2* vec);

	//CVec2* Divide(CVec2* vec);
};

typedef CVec2* Vec2;

