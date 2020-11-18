#include <Windows.h>
#include "StopWatch.h"

Stopwatch::Stopwatch()
{
	Reset();
}

void Stopwatch::Start()
{
	if (!isRunning)
	{
		isRunning = true;
		startTimeStamp = (long) GetTickCount64();
	}
}

void Stopwatch::Stop()
{
	if (isRunning)
	{
		long endTimeStamp = (long) GetTickCount64();
		long elapsedPeriod = endTimeStamp - startTimeStamp;
		elapsed += elapsedPeriod;
		isRunning = false;
	}
}

void Stopwatch::Reset()
{
	elapsed = 0;
	startTimeStamp = 0;
	isRunning = false;
}

void Stopwatch::Restart()
{
	Reset();
	Start();
}

bool Stopwatch::IsRunning()
{
	return isRunning;
}

long Stopwatch::Elapsed()
{
	return GetElapsedTime();
}

long Stopwatch::GetElapsedTime()
{
	long timeElapsed = elapsed;

	if (isRunning)
	{
		long currentTimeStamp = (long) GetTickCount64();
		long elapsedUntilNow = currentTimeStamp - startTimeStamp;
		timeElapsed += elapsedUntilNow;
	}
	return timeElapsed;
}
