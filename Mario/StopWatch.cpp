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
		startTimeStamp = std::chrono::high_resolution_clock::now();
	}
}

void Stopwatch::Stop()
{
	if (isRunning)
	{
		std::chrono::steady_clock::time_point endTimeStamp = std::chrono::high_resolution_clock::now();
		long elapsedPeriod = (long) std::chrono::duration_cast<std::chrono::milliseconds>(endTimeStamp - startTimeStamp).count();
		elapsed += elapsedPeriod;
		isRunning = false;
	}
}

void Stopwatch::Reset()
{
	elapsed = 0;
	startTimeStamp = std::chrono::high_resolution_clock::now();
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
		std::chrono::steady_clock::time_point currentTimeStamp = std::chrono::high_resolution_clock::now();
		long elapsedUntilNow = (long) std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeStamp - startTimeStamp).count();
		timeElapsed += elapsedUntilNow;
	}
	return timeElapsed;
}
