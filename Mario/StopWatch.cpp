#include <Windows.h>
#include "StopWatch.h"
#include "Game.h"

Stopwatch::Stopwatch()
{
	Reset();
}

Stopwatch::Stopwatch(TimeMode mode)
{
	this->mode = mode;
	Reset();
}

void Stopwatch::Start()
{
	if (!isRunning)
	{
		isRunning = true;
		startTimeStamp = std::chrono::high_resolution_clock::now();

		if (mode == TimeMode::GAME_TIME) {
			startTime = CGame::Time().TotalGameTime;
		}
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

		if (mode == TimeMode::GAME_TIME) {
			elapsed -= elapsedPeriod;
			elapsed += CGame::Time().TotalGameTime - startTime;
		}
	}
}

void Stopwatch::Reset()
{
	elapsed = 0;
	startTimeStamp = std::chrono::high_resolution_clock::now();
	isRunning = false;

	if (mode == TimeMode::GAME_TIME) {
		startTime = CGame::Time().TotalGameTime;
	}
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

void Stopwatch::SetTimeMode(TimeMode mode)
{
	this->mode = mode;
}

TimeMode Stopwatch::GetTimeMode()
{
	return mode;
}

long Stopwatch::GetElapsedTime()
{
	long timeElapsed = elapsed;

	if (isRunning)
	{
		std::chrono::steady_clock::time_point currentTimeStamp = std::chrono::high_resolution_clock::now();
		long elapsedUntilNow = (long) std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeStamp - startTimeStamp).count();
		timeElapsed += elapsedUntilNow;

		if (mode == TimeMode::GAME_TIME) {
			timeElapsed = elapsed;
			timeElapsed += CGame::Time().TotalGameTime - startTime;
		}
	}
	return timeElapsed;
}
