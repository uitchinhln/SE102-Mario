#pragma once

#include "TimeSpan.h"

class GameTime
{
	long previousTicks = 0;

public:
	TimeSpan TotalGameTime = TimeSpan::Zero();
	TimeSpan ElapsedGameTime = TimeSpan::Zero();

	long GetPreviousTicks()
	{
		return previousTicks;
	}

	void SetPreviousTicks(long tick)
	{
		previousTicks = tick;
	}
};