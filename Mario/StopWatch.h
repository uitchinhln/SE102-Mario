#pragma once
#include <chrono>

enum class TimeMode {
	SYSTEM_TIME,
	GAME_TIME
};

class Stopwatch
{
public:
	Stopwatch();

	Stopwatch(TimeMode mode);

	virtual void Start();
	virtual void Stop();
	virtual void Reset();
	virtual void Restart();

	virtual bool IsRunning();

	virtual long Elapsed();

	virtual void SetTimeMode(TimeMode mode);

	virtual TimeMode GetTimeMode();

protected:
	long elapsed;
	std::chrono::steady_clock::time_point startTimeStamp;

	long startTime;

	bool isRunning;

	TimeMode mode = TimeMode::GAME_TIME;

	long GetElapsedTime();
};