#pragma once
class Position
{
protected:
	int x;
	int y;

public:
	Position(int x = 0, int y = 0);

	int GetX();
	void SetX(int x);

	int GetY();
	void SetY(int y);
};

typedef Position* LPPosition;

