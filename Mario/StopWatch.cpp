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
		startTimeStamp = GetTickCount();
	}
}

void Stopwatch::Stop()
{
	if (isRunning)
	{
		DWORD endTimeStamp = GetTickCount();
		DWORD elapsedPeriod = endTimeStamp - startTimeStamp;
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
	DWORD timeElapsed = elapsed;

	if (isRunning)
	{
		DWORD currentTimeStamp = GetTickCount();
		DWORD elapsedUntilNow = currentTimeStamp - startTimeStamp;
		timeElapsed += elapsedUntilNow;
	}
	return timeElapsed;
}
