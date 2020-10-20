#pragma once
#include "Utils.h"

class GameTime
{
	long previousTicks = 0;

public:
	DWORD TotalGameTime = 0;
	DWORD ElapsedGameTime = 0;

	long GetPreviousTicks()
	{
		return previousTicks;
	}

	void SetPreviousTicks(long tick)
	{
		previousTicks = tick;
	}
};